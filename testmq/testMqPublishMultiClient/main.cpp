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
#include "AtomicInteger.hpp"

using namespace obotcha;
using namespace gagira;

CountDownLatch latch = createCountDownLatch(1);

//AtomicInteger times = createAtomicInteger(0);

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
            TEST_FAIL("testMqSendOneShotMultiClients case1,name is %s,age is %d",info->name->toChars(),info->age);
        }
        times++;
        return false;
    }

    int times = 0;
};

int main() {
    
    MqCenterBuilder builder = createMqCenterBuilder();
    builder->setUrl("tcp://127.0.0.1:1320")
           ->setRedeliveryInterval(3);

    MqCenter center = builder->build();
    
    ConnectionListener listener1 = createConnectionListener();
    MqConnection connection1 = createMqConnection("tcp://127.0.0.1:1320");
    connection1->connect();
    connection1->subscribe("info",listener1);

    ConnectionListener listener2 = createConnectionListener();
    MqConnection connection2 = createMqConnection("tcp://127.0.0.1:1320");
    connection2->connect();
    connection2->subscribe("info",listener2);

    ConnectionListener listener3 = createConnectionListener();
    MqConnection connection3 = createMqConnection("tcp://127.0.0.1:1320");
    connection3->connect();
    connection3->subscribe("info",listener3);

    StudentInfo student = createStudentInfo();
    student->name = createString("wang");
    student->age = 12;
    //connection->publish("info",student,st(MqMessage)::PublishOneShot);
    for(int i = 0;i < 1024*32;i++) {
        connection1->publish("info",student,st(MqMessage)::Publish);
    }

    sleep(100);

    int total = listener1->times + listener2->times + listener3->times;
    if(total != 1024*32*3) {
        TEST_FAIL("testSendOneShotMultiClients case1,total is %d,l1 is %d,l2 is %d,l3 is %d,expected result is %d",
                    total,listener1->times,listener2->times,listener3->times,1024*32);
    }

    TEST_OK("testSendOneShotMultiClients case100");
    return 0;
}

