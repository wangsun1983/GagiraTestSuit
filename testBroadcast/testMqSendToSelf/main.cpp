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

using namespace obotcha;
using namespace gagira;

DECLARE_CLASS(StudentInfo) IMPLEMENTS(Serializable){
public:
    String name;
    int age;
    DECLARE_REFLECT_FIELD(StudentInfo,name,age);
};

CountDownLatch latch1 = CountDownLatch::New(1);
CountDownLatch latch2 = CountDownLatch::New(1);

DECLARE_CLASS(ConnectionListener1) IMPLEMENTS(BroadcastConnectionListener) {
public:
    int onMessage(String channel,ByteArray data) {
        TEST_FAIL("testMqSendToSelf case1");
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

DECLARE_CLASS(ConnectionListener2) IMPLEMENTS(BroadcastConnectionListener) {
public:
    int onMessage(String channel,ByteArray data) {
        StudentInfo info = StudentInfo::New();
        info->deserialize(data);
        if(!info->name->equals("wang") && info->age != 12) {
            TEST_FAIL("testMqSendToSelf case2,name is %s,age is %d",info->name->toChars(),info->age);
        }
        latch1->countDown();
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
    String url = String::New("tcp://127.0.0.1:")->append(String::New(port));

    DistributeCenterBuilder builder = DistributeCenterBuilder::New();
    builder->setUrl(url);
    DistributeOption option = DistributeOption::New();
    BroadcastCenter center = builder->build();
    int ret = center->start();
    
    BroadcastConnection connection = BroadcastConnection::New(url,ConnectionListener::New1());
    connection->connect();
    connection->subscribeChannel("info");
    
    BroadcastConnection connection2 = BroadcastConnection::New(url,ConnectionListener::New2());
    connection2->connect();
    connection2->subscribeChannel("info");

    StudentInfo student = StudentInfo::New();
    student->name = String::New("wang");
    student->age = 12;
    connection->publishMessage("info",student);
 
    latch1->await();
    usleep(1000*100);
    TEST_OK("testMqSendToSelf case100");
    
    return 0;
}
