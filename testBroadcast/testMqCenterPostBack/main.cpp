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
#include "MqSustainMessage.hpp"

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
            TEST_FAIL("testmqsend case1,name is %s,age is %d",info->name->toChars(),info->age);
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

    void onSustain(int code,String msg) {
      //TODO
    }
};

int main() {

    int port = getEnvPort();
    String url = String::New("tcp://127.0.0.1:")->append(String::New(port));

    MqCenterBuilder builder = createMqCenterBuilder();
    builder->setUrl(url);
    MqOption option = createMqOption();
    option->setWaitPostBack(true);
    builder->setOption(option);
    MqCenter center = builder->build();
    int ret = center->start();
    printf("mqsend ret is %d \n",ret);
    MqConnection connection = createMqConnection(url);
    connection->connect();
    usleep(1000*100);
    connection->subscribePersistenceChannel();

    StudentInfo student = createStudentInfo();
    student->name = String::New("wang");
    student->age = 12;

    MqMessage msg = createMqMessage(String::New("info"),student->serialize(),
                    st(MqMessage)::Publish|st(MqMessage)::StickFlag);
    printf("send message trace1 \n");
    connection->postBackMessage(msg->generatePacket());

    MqMessage msg2 = createMqMessage(nullptr,nullptr,st(MqMessage)::CompleteFlag);
    printf("send message trace2 \n");
    connection->postBackMessage(msg2->generatePacket(),st(MqMessage)::CompleteFlag);
    //start send
    usleep(1000 * 100);
    
    MqConnection connection2 = createMqConnection(url,createConnectionListener());
    connection2->connect();
    usleep(1000*100);
    printf("send message trace2 \n");
    connection2->subscribeChannel(String::New("info"));
    latch->await();

    TEST_OK("testMqCenterWaitPostBack case100");
    return 0;
}
