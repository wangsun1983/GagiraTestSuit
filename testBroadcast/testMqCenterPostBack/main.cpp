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
#include "BroadcastSustainMessage.hpp"

using namespace obotcha;
using namespace gagira;

CountDownLatch latch = CountDownLatch::New(1);

DECLARE_CLASS(StudentInfo) IMPLEMENTS(Serializable){
public:
    String name;
    int age;
    DECLARE_REFLECT_FIELD(StudentInfo,name,age);
};

DECLARE_CLASS(MyConnectionListener) IMPLEMENTS(BroadcastConnectionListener) {
public:
    int onMessage(String channel,ByteArray data) {
        StudentInfo info = StudentInfo::New();
        info->deserialize(data);
        if(!info->name->sameAs("wang") && info->age != 12) {
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

    DistributeCenterBuilder builder = DistributeCenterBuilder::New();
    builder->setUrl(url);
    DistributeOption option = DistributeOption::New();
    option->setWaitPostBack(true);
    builder->setOption(option);
    BroadcastCenter center = builder->buildBroadcastCenter();
    int ret = center->start();
    BroadcastConnection connection = BroadcastConnection::New(url);
    connection->connect();
    usleep(1000*100);
    connection->subscribePersistenceChannel();

    StudentInfo student = StudentInfo::New();
    student->name = String::New("wang");
    student->age = 12;

    BroadcastMessage msg = BroadcastMessage::New(String::New("info"),student->serialize(),
                    st(BroadcastMessage)::Publish|st(BroadcastMessage)::StickFlag);
    connection->postBackMessage(msg->serialize());

    BroadcastMessage msg2 = BroadcastMessage::New(nullptr,nullptr,st(BroadcastMessage)::CompleteFlag);
    connection->postBackMessage(msg2->serialize(),st(BroadcastMessage)::CompleteFlag);
    //start send
    usleep(1000 * 100);
    
    BroadcastConnection connection2 = BroadcastConnection::New(url,MyConnectionListener::New());
    connection2->connect();
    usleep(1000*100);
    connection2->subscribeChannel(String::New("info"));
    latch->await();

    TEST_OK("testBroadcastCenterWaitPostBack case100");
    return 0;
}
