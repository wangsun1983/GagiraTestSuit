#include <stdio.h>
#include <unistd.h>

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
    bool onEvent(String channel,ByteArray data) {
        StudentInfo info = createStudentInfo();
        info->deserialize(data);
        if(!info->name->equals("wang") && info->age != 12) {
            TEST_FAIL("testmqsend case1,name is %s,age is %d",info->name->toChars(),info->age);
        }
        latch->countDown();
        return true;
    }
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
    
    int pid = fork();

    if(pid != 0) {
        sleep(1);
        MqConnection connection = createMqConnection("tcp://127.0.0.1:1250");
        connection->connect();
        connection->subscribe("info",createConnectionListener());
        MyHandler h = createMyHandler(latch);
        h->sendEmptyMessageDelayed(1,1*1000);
        latch->await();
        TEST_OK("testmqsend case100");
    } else {
        MqCenterBuilder builder = createMqCenterBuilder();
        builder->setUrl("tcp://127.0.0.1:1250");
        MqCenter center = builder->build();
        MqConnection connection = createMqConnection("tcp://127.0.0.1:1250");
        connection->connect();
        //start send
        for(int i = 0; i <1024*32;i++) {
            StudentInfo student = createStudentInfo();
            student->name = createString("wang");
            student->age = 12;
            connection->publish("info",student,st(MqMessage)::PublishOneShot);
        }

        sleep(5);
    }

    return 0;
}

