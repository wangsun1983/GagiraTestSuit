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
        if(code != st(MqSustainMessage)::WaitForPostBack) {
            TEST_FAIL("testMqCenterWaitPostBack case1");
        }
        latch->countDown();
    }
};

int main() {

    int port = getEnvPort();
    String url = String::New("tcp://127.0.0.1:")->append(String::New(1110));

    MqCenterBuilder builder = createMqCenterBuilder();
    builder->setUrl(url);
    MqOption option = createMqOption();
    option->setWaitPostBack(true);
    builder->setOption(option);
    MqCenter center = builder->build();
    int ret = center->start();
    printf("mqsend ret is %d \n",ret);
    MqConnection connection = createMqConnection(url,createConnectionListener());
    connection->connect();
    //start send
    usleep(1000 * 100);
    
    StudentInfo student = createStudentInfo();
    student->name = String::New("wang");
    student->age = 12;
    connection->publishMessage("info",student,
        createMqMessageParam()->setFlags(st(MqMessage)::OneShotFlag)->build());

    latch->await();

    TEST_OK("testMqCenterWaitPostBack case100");
    return 0;
}
