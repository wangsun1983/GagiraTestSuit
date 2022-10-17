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

CountDownLatch latch1 = createCountDownLatch(1);
CountDownLatch latch2 = createCountDownLatch(1);

int count = 0;
int detach = 0;

DECLARE_CLASS(StudentInfo) IMPLEMENTS(Serializable){
public:
    String name;
    int age;
    DECLARE_REFLECT_FIELD(StudentInfo,name,age);
};

DECLARE_CLASS(ConnectionListener) IMPLEMENTS(MqConnectionListener) {
public:
    bool onMessage(String channel,ByteArray data) {
        count++;
        latch1->countDown();
        return true;
    }

    bool onDisconnect() {
      //TODO
      return false;
    }

    bool onConnect() {
      //TODO
      return false;
    }

    bool onDetach(String channel) {
      detach = 1;
      if(!channel->equals("info")) {
        TEST_FAIL("testMqUnsubscribe detach case2");
      }
      return false;
    }
};

DECLARE_CLASS(MyHandler) IMPLEMENTS (Handler) {
public:
    _MyHandler(CountDownLatch l) {
        latch = l;
    }

    void handleMessage(Message msg) {
        this->sendEmptyMessageDelayed(1,1*1000);
    }

private:
    CountDownLatch latch;
};


int main() {

    int pid = fork();

    if(pid != 0) {
        sleep(1);
        MqConnection connection = createMqConnection("tcp://127.0.0.1:1280",createConnectionListener());
        connection->connect();
        connection->subscribe("info");
        latch1->await();
        connection->unSubscribe("info");
        latch2->await(1000*5);

        if(count != 1) {
          TEST_FAIL("testMqUnsubscribe case1");
        } else {
          TEST_OK("testMqUnsubscribe case100");
        }

        if(detach != 1) {
          TEST_FAIL("testMqUnsubscribe case2");
        } else {
          TEST_OK("testMqUnsubscribe case101");
        }

    } else {
        MqCenterBuilder builder = createMqCenterBuilder();
        builder->setUrl("tcp://127.0.0.1:1280");
        MqCenter center = builder->build();
        MqConnection connection = createMqConnection("tcp://127.0.0.1:1280");
        connection->connect();
        //start send
        sleep(2);
        //for(int i = 0; i <1024*32;i++) {
        StudentInfo student = createStudentInfo();
        student->name = createString("wang");
        student->age = 12;
        connection->publish("info",student,st(MqMessage)::PublishOneShot);
        //}
        sleep(2);
        StudentInfo student2 = createStudentInfo();
        student2->name = createString("wang");
        student2->age = 12;
        connection->publish("info",student2,st(MqMessage)::PublishOneShot);

        sleep(5);
    }

    return 0;
}
