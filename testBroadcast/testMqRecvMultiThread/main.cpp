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
#include "NetPort.hpp"

using namespace obotcha;
using namespace gagira;

CountDownLatch latch1 = CountDownLatch::New(1);
CountDownLatch latch2 = CountDownLatch::New(1);
CountDownLatch latch3 = CountDownLatch::New(1);

int total = 1024*32;

std::atomic<int> count(0);

int v1 = 0;
int v2 = 0;
int v3 = 0;

DECLARE_CLASS(CountHandler) IMPLEMENTS(Handler) {
public:
    void handleMessage(Message) {
        int v = count;
        printf("v1 is %d,v2 is %d,v3 is %d \n",v1,v2,v3);
        sendEmptyMessageDelayed(1,1000);
    }
};

DECLARE_CLASS(ConnectionListener) IMPLEMENTS(BroadcastConnectionListener) {
public:
    _ConnectionListener(int t) {
        //handler = createCountHandler();
        //handler->sendEmptyMessageDelayed(1,1000);
        type = t;
    }

    int onMessage(String channel,ByteArray data) {
        String str = data->toString();
        if(str->equals("hello world")) {
            count++;
        }

        switch(type) {
            case 1:
                v1++;
                if(v1 == total) {
                    latch1->countDown();
                }
            break;

            case 2:
                v2++;
                if(v2 == total) {
                    latch2->countDown();
                }
            break;

            case 3:
                v3++;
                if(v3 == total) {
                    latch3->countDown();
                }
            break;
        }

        return 0;
    }

    void onDisconnect() {
        
    }

    void onConnect() {
        
    }

    void onDetach(String channel) {
        
    }

    void onSustain(int code,String msg){}

private:
    CountHandler handler;
    int type;
};

int main() {

    int port = getEnvPort();
    String url = String::New("tcp://127.0.0.1:")->append(String::New(port));

    int pid = fork();

    if(pid == 0) {
        sleep(1);
        BroadcastConnection connection1 = BroadcastConnection::New(url,ConnectionListener::New(1));
        connection1->connect();
        connection1->subscribeChannel("info");

        BroadcastConnection connection2 = BroadcastConnection::New(url,ConnectionListener::New(2));
        connection2->connect();
        connection2->subscribeChannel("info");

        BroadcastConnection connection3 = BroadcastConnection::New(url,ConnectionListener::New(3));
        connection3->connect();
        connection3->subscribeChannel("info");

        CountHandler handler = createCountHandler();
        handler->sendEmptyMessageDelayed(1,1000);

        latch1->await();
        latch2->await();
        latch3->await();
        
        if(v1 != total || v2 != total || v3 != total) {
            TEST_FAIL("testMqRecvMultiThread case1");
        } else {
            TEST_OK("testMqRecvMultiThread case100");
        }

    } else {
        DistributeCenterBuilder builder = DistributeCenterBuilder::New();
        builder->setUrl(url);
        BroadcastCenter center = builder->build();
        center->start();
        BroadcastConnection connection = BroadcastConnection::New(url);
        connection->connect();
        sleep(2);
        String str = String::New("hello world");   
        ByteArray data = str->toByteArray();
        BroadcastMessageParam param = BroadcastMessage::NewParam();
        param->setFlags(st(BroadcastMessage)::Publish);

        for(int i = 0;i < total;i++) {
            connection->publishMessage("info",data,param);
        }

        int result = 0;
        wait(&result);

        port++;
        setEnvPort(port);
        TEST_OK("testMqRecvMultiThread case101");
    }

    return 0;
}
