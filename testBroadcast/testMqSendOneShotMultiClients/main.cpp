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
#include "NetPort.hpp"

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
    int onMessage(String channel,ByteArray data) {
        StudentInfo info = createStudentInfo();
        info->deserialize(data);
        if(!info->name->equals("wang") && info->age != 12) {
            TEST_FAIL("testMqSendOneShotMultiClients case1,name is %s,age is %d",info->name->toChars(),info->age);
        }
        times++;
        return 1;
    }

    void onDisconnect(){};
    void onConnect(){};
    void onDetach(String channel){};
    void onSustain(int code,String msg){}

    int times = 0;
};

int main() {
    
    int port = getEnvPort();
    String url = createString("tcp://127.0.0.1:")->append(createString(port));

    MqCenterBuilder builder = createMqCenterBuilder();
    builder->setUrl(url);

    MqCenter center = builder->build();
    center->start();
    
    ConnectionListener listener1 = createConnectionListener();
    MqConnection connection1 = createMqConnection(url,listener1);
    connection1->connect();
    connection1->subscribeChannel("info");

    ConnectionListener listener2 = createConnectionListener();
    MqConnection connection2 = createMqConnection(url,listener2);
    connection2->connect();
    connection2->subscribeChannel("info");

    ConnectionListener listener3 = createConnectionListener();
    MqConnection connection3 = createMqConnection(url,listener3);
    connection3->connect();
    connection3->subscribeChannel("info");
    usleep(1000 * 100);

    StudentInfo student = createStudentInfo();
    student->name = createString("wang");
    student->age = 12;
    auto param = createMqMessageParam();
    param->setFlags(st(MqMessage)::OneShotFlag);
    for(int i = 0;i < 1024*32;i++) {
        connection1->publishMessage("info",student,param);
    }

    sleep(30);

    int total = listener1->times + listener2->times + listener3->times;
    if(total != 1024*32) {
        TEST_FAIL("testSendOneShotMultiClients case1,total is %d,l1 is %d,l2 is %d,l3 is %d,expected result is %d",
                    total,listener1->times,listener2->times,listener3->times,1024*32);
    }

    port++;
    setEnvPort(port);
    TEST_OK("testSendOneShotMultiClients case100");
    return 0;
}

