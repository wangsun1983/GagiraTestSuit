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

CountDownLatch latch1 = createCountDownLatch(1);
CountDownLatch latch2 = createCountDownLatch(1);
CountDownLatch latch3 = createCountDownLatch(1);

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

DECLARE_CLASS(ConnectionListener) IMPLEMENTS(MqConnectionListener) {
public:
    _ConnectionListener(int t) {
        //handler = createCountHandler();
        //handler->sendEmptyMessageDelayed(1,1000);
        type = t;
    }

    bool onMessage(String channel,ByteArray data) {
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


        return true;
    }

    bool onDisconnect() {
        return false;
    }

    bool onConnect() {
        return false;
    }

    bool onDetach(String channel) {
        return false;
    }

private:
    CountHandler handler;
    int type;
};

int main() {

    int pid = fork();

    if(pid == 0) {
        sleep(1);
        MqConnection connection1 = createMqConnection("tcp://127.0.0.1:1800",createConnectionListener(1));
        connection1->connect();
        connection1->subscribe("info");

        MqConnection connection2 = createMqConnection("tcp://127.0.0.1:1800",createConnectionListener(2));
        connection2->connect();
        connection2->subscribe("info");

        MqConnection connection3 = createMqConnection("tcp://127.0.0.1:1800",createConnectionListener(3));
        connection3->connect();
        connection3->subscribe("info");

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
        MqCenterBuilder builder = createMqCenterBuilder();
        builder->setUrl("tcp://127.0.0.1:1800");
        MqCenter center = builder->build();
        MqConnection connection = createMqConnection("tcp://127.0.0.1:1800");
        connection->connect();
        sleep(2);
        String str = createString("hello world");   
        ByteArray data = str->toByteArray();

        for(int i = 0;i < total;i++) {
            connection->publish("info",data,st(MqMessage)::Publish);
        }

        int result = 0;
        wait(&result);
        TEST_OK("testMqRecvMultiThread case101");
    }

    return 0;
}
