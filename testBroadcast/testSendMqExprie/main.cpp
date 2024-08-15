#include <stdio.h>
#include <unistd.h>

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
#include "MqDLQMessage.hpp"
#include "System.hpp"

using namespace obotcha;
using namespace gagira;

CountDownLatch latch = CountDownLatch::New(1);

DECLARE_CLASS(StudentInfo) IMPLEMENTS(Serializable){
public:
    String name;
    int age;
    DECLARE_REFLECT_FIELD(StudentInfo,name,age);
};

DECLARE_CLASS(DLQListener) IMPLEMENTS(BroadcastConnectionListener) {
public:
    int onMessage(String channel,ByteArray data) {
        MqDLQMessage message = createMqDLQMessage();
        message->deserialize(data);
        if(message->getCode() != st(MqDLQMessage)::MessageTimeOut) {
            TEST_FAIL("test MqExpire send case1");
        }

        long current = st(System)::CurrentTimeMillis();
        long time = message->getPointTime();
        if(current - time > 10 || current - time < -5) {
            TEST_FAIL("test MqExpire send case2");
        }

        printf("token is %s \n",message->getToken()->toChars());

        BroadcastMessage BroadcastMessage = st(BroadcastMessage)::generateMessage(message->getData());
        StudentInfo info = StudentInfo::New();
        info->deserialize(BroadcastMessage->getData());
        if(!info->name->equals("wang") && info->age != 12) {
            TEST_FAIL("test MqExpire send case3");
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
    String url = String::New("tcp://127.0.0.1:")->append(String::New(1424));
    DistributeCenterBuilder builder = DistributeCenterBuilder::New();
    builder->setUrl(url);
    BroadcastCenter center = builder->build();
    int ret = center->start();


    BroadcastConnection connection = BroadcastConnection::New(url,createDLQListener());
    connection->connect();
    connection->subscribeDLQChannel();

    usleep(1000*50);
    BroadcastConnection connection2 = BroadcastConnection::New(url);
    connection2->connect();
    StudentInfo student = StudentInfo::New();
    student->name = String::New("wang");
    student->age = 12;
    connection2->publishMessage("info",student,
            BroadcastMessage::NewParam()->setTTL(1)->build());
    usleep(1000 * 50);
    latch->await();

    TEST_OK("test MqExpire send case3");
    return 0;
}
