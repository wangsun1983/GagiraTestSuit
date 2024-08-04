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

CountDownLatch latch = createCountDownLatch(4);

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
        return st(MqConnectionListener)::AutoAck;
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

    int pid = fork();

    if(pid != 0) {
        usleep(1000*100);
        MqConnection connection = createMqConnection("tcp://127.0.0.1:1300",createConnectionListener());
        connection->connect();
        connection->subscribeChannel("info");
        latch->await(1000*5);
        if(latch->getCount() != 3) {
            TEST_FAIL("testmqsendAcknowledge resp case1,count is %d",latch->getCount());
        }
        TEST_OK("testmqsendAcknowledge resp case100");
    } else {
        MqCenterBuilder builder = createMqCenterBuilder();
        builder->setUrl("tcp://127.0.0.1:1300");
        MqCenter center = builder->build();
        int ret = center->start();
        //printf("mqsend ret is %d \n",ret);
        MqConnection connection = createMqConnection("tcp://127.0.0.1:1300");
        connection->connect();
        //start send
        usleep(1000*200);
        StudentInfo student = createStudentInfo();
        student->name = String::New("wang");
        student->age = 12;
        MqMessageParam param = createMqMessageParam();
        param->setFlags(st(MqMessage)::OneShotFlag|st(MqMessage)::AcknowledgeFlag);
        connection->publishMessage("info",student,param);
        sleep(6);
    }

    return 0;
}
