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

DECLARE_CLASS(ConnectionListener) IMPLEMENTS(BroadcastConnectionListener) {
public:
    int onMessage(String channel,ByteArray data) {
        //TODO
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
        printf("on sustain \n");
        if(code != st(BroadcastSustainMessage)::WaitForPostBack) {
            TEST_FAIL("testBroadcastCenterWaitPostBack case1");
        }
        latch->countDown();
    }
};

int main() {

    int port = getEnvPort();
    String url = String::New("tcp://127.0.0.1:")->append(String::New(1110));

    DistributeCenterBuilder builder = DistributeCenterBuilder::New();
    builder->setUrl(url);
    DistributeOption option = DistributeOption::New();
    option->setWaitPostBack(true);
    builder->setOption(option);
    BroadcastCenter center = builder->build();
    int ret = center->start();
    printf("mqsend ret is %d \n",ret);
    BroadcastConnection connection = BroadcastConnection::New(url,ConnectionListener::New());
    connection->connect();
    //start send
    usleep(1000 * 100);
    
    StudentInfo student = StudentInfo::New();
    student->name = String::New("wang");
    student->age = 12;
    connection->publishMessage("info",student,
        BroadcastMessage::NewParam()->setFlags(st(BroadcastMessage)::OneShotFlag)->build());

    latch->await();

    TEST_OK("testBroadcastCenterWaitPostBack case100");
    return 0;
}
