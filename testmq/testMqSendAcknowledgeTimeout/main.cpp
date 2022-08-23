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

CountDownLatch latch = createCountDownLatch(1);

int times = 0;

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
            TEST_FAIL("testMqSendBeforeSubscribe case1,name is %s,age is %d",info->name->toChars(),info->age);
        }
        times++;
        return false;
    }
};

int main() {
    
    MqCenterBuilder builder = createMqCenterBuilder();
    builder->setUrl("tcp://127.0.0.1:1300")
           ->setRedeliveryInterval(3);

    MqCenter center = builder->build();
    
    sleep(1);
    MqConnection connection = createMqConnection("tcp://127.0.0.1:1300");
    StudentInfo student = createStudentInfo();
    student->name = createString("wangsl");
    student->age = 12;
    
    connection->connect();
    //start send
    connection->publish("info",student,st(MqMessage)::PublishOneShot|st(MqMessage)::Acknowledge);

    MqConnection connection2 = createMqConnection("tcp://127.0.0.1:1300");
    
    connection2->connect();
    connection2->subscribe("info",createConnectionListener());
    sleep(10);
    if(times != 4) {
        TEST_FAIL("testMqSendBeforeSubscribe case1,times is %d",times);
    }
    
    TEST_OK("testMqSendBeforeSubscribe case100");
    return 0;
}

