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
#include "NetPort.hpp"
#include "TimeWatcher.hpp"

using namespace obotcha;
using namespace gagira;

CountDownLatch latch = createCountDownLatch(1);

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
            TEST_FAIL("testMqSendDelayed case1,name is %s,age is %d",info->name->toChars(),info->age);
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

int main() {
    int port = getEnvPort();
    String url = createString("tcp://127.0.0.1:")->append(createString(1464));
    MqCenterBuilder builder = createMqCenterBuilder();
    builder->setUrl(url);
    MqCenter center = builder->build();
    int ret = center->start();
    MqConnection connection = createMqConnection(url,createConnectionListener());
    connection->connect();
    connection->subscribeChannel("info");

    StudentInfo student = createStudentInfo();
    student->name = createString("wang");
    student->age = 12;
    MqMessageParam param = createMqMessageParam();
    param->setDelayInterval(300);
    TimeWatcher watch = createTimeWatcher();
    MqConnection connection2 = createMqConnection(url);
    connection2->connect();

    watch->start();
    connection2->publishMessage("info",student,param);
    latch->await();
    long waitresult = watch->stop();

    if(waitresult > 310 || waitresult < 300) {
        TEST_FAIL("testMqSendDelayed case2,waitresult is %d",waitresult);
    }

    TEST_OK("testMqSendDelayed case100");
    return 0;
}
