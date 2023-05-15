#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#include "Controller.hpp"
#include "Server.hpp"
#include "Inet4Address.hpp"
#include "ServletRequest.hpp"
#include "HttpResourceManager.hpp"
#include "Reflect.hpp"
#include "Utils.hpp"
#include "MqCenter.hpp"
#include "MqConnection.hpp"
#include "Serializable.hpp"
#include "CountDownLatch.hpp"
#include "TestLog.hpp"
#include "MqCenterBuilder.hpp"
#include "Handler.hpp"
#include "NetPort.hpp"

using namespace obotcha;
using namespace gagira;

CountDownLatch latch = createCountDownLatch(1024*32);

DECLARE_CLASS(StudentInfo) IMPLEMENTS(Serializable){
public:
    String name;
    int age;
    DECLARE_REFLECT_FIELD(StudentInfo,name,age);
};

DECLARE_CLASS(ConnectionListener) IMPLEMENTS(MqConnectionListener) {
public:
    int onMessage(String channel,ByteArray data) {
        StudentInfo info = createStudentInfo();
        info->deserialize(data);
        if(!info->name->equals("wang") && info->age != 12) {
            TEST_FAIL("testmqsendPersist no Msg case1,name is %s,age is %d",info->name->toChars(),info->age);
        }
        latch->countDown();
        return 0;
    }

    void onDisconnect() {
      //TODO
      //return false;
    }

    void onConnect() {
      //TODO
      //return false;
    }

    void onDetach(String channel) {
      //TODO
      //return false;
    }
    void onSustain(int code,String msg){}
};

AtomicInteger persistentCount = createAtomicInteger(0);
DECLARE_CLASS(PersistentConnectionListener) IMPLEMENTS(MqConnectionListener) {
public:
    int onMessage(String channel,ByteArray data) {
        StudentInfo info = createStudentInfo();
        info->deserialize(data);
        if(!info->name->equals("wang") && info->age != 12) {
            TEST_FAIL("testmqsendPersist no Msg case1,name is %s,age is %d",info->name->toChars(),info->age);
        }
        persistentCount->incrementAndGet();
        return 0;
    }

    void onDisconnect() {
      //TODO
      //return false;
    }

    void onConnect() {
      //TODO
      //return false;
    }

    void onDetach(String channel) {
      //TODO
      //return false;
    }

    void onSustain(int code,String msg){}
};

DECLARE_CLASS(MyHandler) IMPLEMENTS (Handler) {
public:
    _MyHandler(CountDownLatch l) {
        latch = l;
    }

    void handleMessage(Message msg) {
        printf("latch count is %d \n",latch->getCount());
        this->sendEmptyMessageDelayed(1,1*1000);
    }

private:
    CountDownLatch latch;
};


int main() {

    int port = getEnvPort();
    String url = createString("tcp://127.0.0.1:")->append(createString(port));

    int pid = fork();

    if(pid != 0) {
        MqCenterBuilder builder = createMqCenterBuilder();
        builder->setUrl(url);
        MqCenter center = builder->build();
        int ret = center->start();
        printf("mqsend ret is %d \n",ret);
        MqConnection connection = createMqConnection(url);
        connection->connect();

        MqConnection dataconnection = createMqConnection(url,createPersistentConnectionListener());
        dataconnection->connect();
        dataconnection->subscribePersistenceChannel();
        //start send
        sleep(2);
        for(int i = 0; i <1024*32;i++) {
            StudentInfo student = createStudentInfo();
            student->name = createString("wang");
            student->age = 12;
            connection->publishMessage("info",student,
                createMqMessageParam()->setFlags(st(MqMessage)::OneShotFlag)->build());
        }
        sleep(5);

        int result;
        wait(&result);

        if(persistentCount->get() != 0) {
            TEST_FAIL("testmqsendPersist no Msg case3 count is %d",persistentCount->get());
        }
        TEST_OK("testmqsendPersist no Msg case101");
    } else {
        sleep(1);
        MqConnection connection = createMqConnection(url,createConnectionListener());
        connection->connect();
        connection->subscribeChannel("info");
        MyHandler h = createMyHandler(latch);
        h->sendEmptyMessageDelayed(1,1*1000);
        latch->await();
        setEnvPort(++port);
        TEST_OK("testmqsendPersist no Msg case100");
    }

    return 0;
}
