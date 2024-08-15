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

CountDownLatch latch = CountDownLatch::New(1);

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
            TEST_FAIL("testMqSendStick case1,name is %s,age is %d",info->name->toChars(),info->age);
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
        DistributeCenterBuilder builder = DistributeCenterBuilder::New();
        builder->setUrl(url);
        BroadcastCenter center = builder->build();
        center->start();
        BroadcastConnection connection = BroadcastConnection::New(url);
        connection->connect();
        
        StudentInfo student = StudentInfo::New();
        student->name = String::New("wang");
        student->age = 12;
        printf("send stick message");
        connection->publishStickMessage("info",student);

        int result = 0;
        wait(&result);
        TEST_OK("testMqSendStick case101");
    } else {
      sleep(1);
      BroadcastConnection connection = BroadcastConnection::New(url,ConnectionListener::New());
      connection->connect();
      connection->subscribeChannel("info");
      latch->await();
      TEST_OK("testMqSendStick case100");
      port++;
      setEnvPort(port);
    }

    return 0;
}
