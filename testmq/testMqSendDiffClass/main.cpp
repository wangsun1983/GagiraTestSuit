#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

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

int total = 1024*32;
CountDownLatch latch = createCountDownLatch(1);

DECLARE_CLASS(CountHandler) IMPLEMENTS(Handler) {
public:
    void handleMessage(Message) {
        printf("total is %d \n",total);
        sendEmptyMessageDelayed(1,1000);
    }
};

DECLARE_CLASS(Person) IMPLEMENTS(Serializable) {
public:
    int age = 1;
    DECLARE_REFLECT_FIELD(Person,age);  
};

DECLARE_CLASS(Student) IMPLEMENTS(Serializable){
public:
    int classno = 2;
    ByteArray data;
    DECLARE_REFLECT_FIELD(Student,classno,data);
};


DECLARE_CLASS(ConnectionListener) IMPLEMENTS(MqConnectionListener) {
public:
    _ConnectionListener() {
        handler = createCountHandler();
        handler->sendEmptyMessageDelayed(1,1000);
    }

    bool onEvent(String channel,ByteArray data) {
        Student info = createStudent();
        info->deserialize(data);

        Person p = createPerson();
        p->deserialize(info->data);

        if(info->classno != p->age) {
            TEST_FAIL("MqSendDiffClass case1");
        } else {
            total--;
        }

        if(total == 0) {
            latch->countDown();
            TEST_OK("MqSendDiffClass case2");
        }
        return true;
    }

private:
    CountHandler handler;
};

DECLARE_CLASS(MyHandler) IMPLEMENTS (Handler) {
public:
    _MyHandler(CountDownLatch l) {
        latch = l;
    }

    void handleMessage(Message msg) {
        printf("latch count is %d \n",latch->getCount());
        this->sendEmptyMessageDelayed(1,1*1000);
    }

private:
    CountDownLatch latch;
};


int main() {
    
    int pid = fork();

    if(pid != 0) {
        sleep(1);
        MqConnection connection = createMqConnection("tcp://127.0.0.1:1250");
        connection->connect();
        connection->subscribe("info",createConnectionListener());
        MyHandler h = createMyHandler(latch);
        h->sendEmptyMessageDelayed(1,1*1000);
        latch->await();
    } else {
        MqCenterBuilder builder = createMqCenterBuilder();
        builder->setUrl("tcp://127.0.0.1:1250");
        MqCenter center = builder->build();
        MqConnection connection = createMqConnection("tcp://127.0.0.1:1250");
        connection->connect();
        sleep(2);
        //start send
        for(int i = 0; i <total;i++) {
            Student student = createStudent();
            student->classno = i;

            Person p = createPerson();
            p->age = i;
            student->data = p->serialize();
            connection->publish("info",student,st(MqMessage)::PublishOneShot);
        }

        int result = 0;
        wait(&result);
        TEST_OK("testmqsendDiffClass case100");
    }

    return 0;
}

