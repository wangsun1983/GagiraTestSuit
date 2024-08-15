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
#include "BroadcastCenter.hpp"
#include "BroadcastConnection.hpp"
#include "Serializable.hpp"
#include "CountDownLatch.hpp"
#include "TestLog.hpp"
#include "DistributeCenterBuilder.hpp"
#include "Handler.hpp"
#include "Process.hpp"
#include "NetPort.hpp"

using namespace obotcha;
using namespace gagira;

int total = 1024*32;
CountDownLatch latch = CountDownLatch::New(1);
CountDownLatch exitlatch = CountDownLatch::New(1);

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


DECLARE_CLASS(ConnectionListener) IMPLEMENTS(BroadcastConnectionListener) {
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
        BroadcastConnection connection = BroadcastConnection::New(url,ConnectionListener::New());
        connection->connect();
        connection->subscribeChannel("info");
        MyHandler h = MyHandler::New(latch);
        h->sendEmptyMessageDelayed(1,1*1000);
        latch->await();
        connection->publishMessage(String::New("close"),String::New("abc"));
    } else {
        DistributeCenterBuilder builder = DistributeCenterBuilder::New();
        builder->setUrl(url);
        BroadcastCenter center = builder->build();
        center->start();
        BroadcastConnection connection = BroadcastConnection::New(url);
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
                BroadcastMessage::NewParam()->setFlags(st(BroadcastMessage)::OneShotFlag)->build());
        }

        exitlatch->await();

        port++;
        setEnvPort(port);
        TEST_OK("testmqsendDiffClass case100");
    }

    return 0;
}

