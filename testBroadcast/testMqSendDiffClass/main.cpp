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
#include "Process.hpp"
#include "NetPort.hpp"

using namespace obotcha;
using namespace gagira;

int total = 1024*32;
CountDownLatch latch = createCountDownLatch(1);
CountDownLatch exitlatch = createCountDownLatch(1);

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

    int onMessage(String channel,ByteArray data) {
        if(channel->equals("close")) {
             exitlatch->countDown();
             return 1;
        }

        //printf("accept message pid is %d \n",st(Process)::myPid());

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
        return 1;
    }

    void onDisconnect(){}
    void onConnect(){}
    void onDetach(String channel){}
    void onSustain(int code,String msg){}

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
    int port = getEnvPort();
    String url = String::New("tcp://127.0.0.1:")->append(String::New(port));

    if(pid != 0) {
        sleep(1);
        MqConnection connection = createMqConnection(url,createConnectionListener());
        connection->connect();
        connection->subscribeChannel("info");
        MyHandler h = MyHandler::New(latch);
        h->sendEmptyMessageDelayed(1,1*1000);
        latch->await();
        connection->publishMessage(String::New("close"),String::New("abc"));
    } else {
        MqCenterBuilder builder = createMqCenterBuilder();
        builder->setUrl(url);
        MqCenter center = builder->build();
        center->start();
        MqConnection connection = createMqConnection(url);
        connection->connect();

        connection->subscribeChannel("close");
        sleep(2);
        //start send
        int i = 100;
        for(int i = 0; i <total;i++) {
            Student student = createStudent();
            student->classno = i;

            Person p = createPerson();
            p->age = i;
            student->data = p->serialize();
            connection->publishMessage("info",student,
                createMqMessageParam()->setFlags(st(MqMessage)::OneShotFlag)->build());
        }

        exitlatch->await();

        port++;
        setEnvPort(port);
        TEST_OK("testmqsendDiffClass case100");
    }

    return 0;
}

