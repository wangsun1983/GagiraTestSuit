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
#include "MqDLQMessage.hpp"
#include "System.hpp"

using namespace obotcha;
using namespace gagira;


CountDownLatch latch = createCountDownLatch(1);

DECLARE_CLASS(StudentInfo) IMPLEMENTS(Serializable){
public:
    String name;
    int age;
    DECLARE_REFLECT_FIELD(StudentInfo,name,age);
};

DECLARE_CLASS(DLQListener) IMPLEMENTS(MqConnectionListener) {
public:
    int onMessage(String channel,ByteArray data) {
        MqDLQMessage message = createMqDLQMessage();
        message->deserialize(data);
        if(message->getCode() != st(MqDLQMessage)::NoClient) {
            TEST_FAIL("test MqDLQ send case1");
        }

        long current = st(System)::currentTimeMillis();
        long time = message->getPointTime();
        if(current - time > 10 || current - time < -5) {
            TEST_FAIL("test MqDLQ send case2");
        }

        //printf("token is %s \n",message->getToken()->toChars());

        MqMessage mqMessage = st(MqMessage)::generateMessage(message->getData());
        StudentInfo info = createStudentInfo();
        info->deserialize(mqMessage->getData());
        if(!info->name->equals("wang") && info->age != 12) {
            TEST_FAIL("test MqDLQ send case3");
        }

        latch->countDown();
        return 1;
    }

    void onDisconnect() {
      //gDisconnect = 100;
      //return false;
    }

    void onConnect() {
      // gConnect = 200;
      //return false;
    }

    void onDetach(String channel) {
      //TODO
      //return false;
    }

    void onSustain(int code,String msg) {

    }
};

int main() {
    int port = getEnvPort();
    String url = createString("tcp://127.0.0.1:")->append(createString(1414));
    MqCenterBuilder builder = createMqCenterBuilder();
    builder->setUrl(url);
    MqCenter center = builder->build();
    int ret = center->start();


    MqConnection connection = createMqConnection(url,createDLQListener());
    connection->connect();
    connection->subscribeDLQChannel();

    usleep(1000*50);
    MqConnection connection2 = createMqConnection(url);
    connection2->connect();
    StudentInfo student = createStudentInfo();
    student->name = createString("wang");
    student->age = 12;
    connection2->publishMessage("info",student);
    usleep(1000 * 50);
    latch->await();
    TEST_OK("test MqDLQ send case100");
    return 0;
}
