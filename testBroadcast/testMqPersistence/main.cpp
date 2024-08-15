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
#include "BroadcastCenter.hpp"
#include "BroadcastConnection.hpp"
#include "Serializable.hpp"
#include "CountDownLatch.hpp"
#include "TestLog.hpp"
#include "DistributeCenterBuilder.hpp"
#include "Handler.hpp"
#include "NetPort.hpp"

using namespace obotcha;
using namespace gagira;

CountDownLatch latch = CountDownLatch::New(1024*32);

DECLARE_CLASS(StudentInfo) IMPLEMENTS(Serializable){
public:
    String name;
    int age;
    DECLARE_REFLECT_FIELD(StudentInfo,name,age);
};

DECLARE_CLASS(ConnectionListener) IMPLEMENTS(BroadcastConnectionListener) {
public:
    int onMessage(String channel,ByteArray data) {
        StudentInfo info = StudentInfo::New();
        info->deserialize(data);
        if(!info->name->equals("wang") && info->age != 12) {
            TEST_FAIL("testmqsendPersistMsg case1,name is %s,age is %d",info->name->toChars(),info->age);
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
DECLARE_CLASS(PersistentConnectionListener) IMPLEMENTS(BroadcastConnectionListener) {
public:
    int onMessage(String channel,ByteArray data) {
        StudentInfo info = StudentInfo::New();
        info->deserialize(data);
        if(!info->name->equals("wang") && info->age != 12) {
            TEST_FAIL("testmqsendPersistMsg case1,name is %s,age is %d",info->name->toChars(),info->age);
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
    String url = String::New("tcp://127.0.0.1:")->append(String::New(port));

    int pid = fork();

    if(pid != 0) {
        sleep(1);
        BroadcastConnection connection = BroadcastConnection::New(url,ConnectionListener::New());
        connection->connect();
        connection->subscribeChannel("info");
        MyHandler h = MyHandler::New(latch);
        h->sendEmptyMessageDelayed(1,1*1000);
        latch->await();
        setEnvPort(++port);
        TEST_OK("testmqsendPersistMsg case100");
    } else {
        DistributeCenterBuilder builder = DistributeCenterBuilder::New();
        builder->setUrl(url);
        BroadcastCenter center = builder->build();
        int ret = center->start();
        printf("mqsend ret is %d \n",ret);
        BroadcastConnection connection = BroadcastConnection::New(url);
        connection->connect();

        BroadcastConnection dataconnection = BroadcastConnection::New(url,createPersistentConnectionListener());
        dataconnection->connect();
        dataconnection->subscribePersistenceChannel();
        //start send
        sleep(2);
        for(int i = 0; i <1024*32;i++) {
            StudentInfo student = StudentInfo::New();
            student->name = String::New("wang");
            student->age = 12;
            BroadcastMessageParam param = BroadcastMessage::NewParam();
            param->setFlags(st(BroadcastMessage)::OneShotFlag|st(BroadcastMessage)::PersistFlag);
            connection->publishMessage("info",student,param);
        }
        sleep(5);

        int result;
        wait(&result);

        if(persistentCount->get() != 1024*32) {
            TEST_FAIL("testmqsendPersistMsg case3 count is %d",persistentCount->get());
        }
        TEST_OK("testmqsendPersistMsg case101");
    }

    return 0;
}
