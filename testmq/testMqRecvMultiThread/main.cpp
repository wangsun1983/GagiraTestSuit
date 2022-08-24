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

CountDownLatch latch = createCountDownLatch(1);
int total = 1024*32;

std::atomic<int> count(0);

int v1 = 0;
int v2 = 0;
int v3 = 0;

DECLARE_CLASS(CountHandler) IMPLEMENTS(Handler) {
public:
    void handleMessage(Message) {
        int v = count;
        printf("total is %d \n",v);
        sendEmptyMessageDelayed(1,1000);
    }
};

DECLARE_CLASS(ConnectionListener) IMPLEMENTS(MqConnectionListener) {
public:
    _ConnectionListener(int t) {
        handler = createCountHandler();
        handler->sendEmptyMessageDelayed(1,1000);
        type = t;
    }

    bool onEvent(String channel,ByteArray data) {
        String str = data->toString();
        if(str->equals("hello world")) {
            count++;
        }

        switch(type) {
            case 1:
                v1++;
            break;

            case 2:
                v2++;
            break;

            case 3:
                v3++;
            break;
        }

        if(count == total*3) {
            TEST_OK("testMqSendLoop case1");
            latch->countDown();
        }

        return true;
    }

private:
    CountHandler handler;
    int type;
};

int main() {
    
    int pid = fork();

    if(pid == 0) {
        sleep(1);
        MqConnection connection = createMqConnection("tcp://127.0.0.1:1260");
        connection->connect();
        connection->subscribe("info",createConnectionListener(1));
        connection->subscribe("info",createConnectionListener(2));
        connection->subscribe("info",createConnectionListener(3));
        latch->await();
        printf("v1 is %d,v2 is %d,v3 is %d \n",v1,v2,v3);

    } else {
        MqCenterBuilder builder = createMqCenterBuilder();
        builder->setUrl("tcp://127.0.0.1:1260");
        MqCenter center = builder->build();
        MqConnection connection = createMqConnection("tcp://127.0.0.1:1260");
        connection->connect();
        sleep(2);
        printf("start !!! \n");
        String str = createString("hello world");
        ByteArray data = str->toByteArray();

        for(int i = 0;i < total;i++) {
            connection->publish("info",data,st(MqMessage)::PublishOneShot);
        }

        int result = 0;
        printf("start wait!!! \n");
        wait(&result);
        TEST_OK("testMqSendLoop case100");
    }

    return 0;
}

