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
int total = 1024*12;

DECLARE_CLASS(CountHandler) IMPLEMENTS(Handler) {
public:
    void handleMessage(Message) {
        printf("total is %d \n",total);
        sendEmptyMessageDelayed(1,1000);
    }
};

DECLARE_CLASS(ConnectionListener) IMPLEMENTS(MqConnectionListener) {
public:
    _ConnectionListener() {
        handler = createCountHandler();
        handler->sendEmptyMessageDelayed(1,1000);
    }

    bool onMessage(String channel,ByteArray data) {
        String str = data->toString();
        if(str->equals("hello world")) {
            total--;
        }

        if(total == 0) {
            TEST_OK("testMqSendLoop case1");
            latch->countDown();
        }

        return true;
    }

    bool onDisconnect() {
        return true;
    }
    
    bool onConnect() {
        return true;
    }

    bool onDetach(String channel) {
        return true;
    }

private:
    CountHandler handler;
};

int main() {
    
    int pid = fork();

    if(pid == 0) {
        sleep(1);
        MqConnection connection = createMqConnection("tcp://127.0.0.1:1270",createConnectionListener());
        connection->connect();
        int count = 0;
        connection->subscribe("info");
        while(count < 1024) {
            connection->unSubscribe("info");
            count++;
        }
        sleep(3);

        if(total != 1024*12) {
            TEST_FAIL("testMqUnSubscribeMore case1");
        } else {
            TEST_OK("testMqUnSubscribeMore case100");
        }
    } else {
        MqCenterBuilder builder = createMqCenterBuilder();
        builder->setUrl("tcp://127.0.0.1:1270");
        MqCenter center = builder->build();
        MqConnection connection = createMqConnection("tcp://127.0.0.1:1270");
        connection->connect();
        sleep(3);
        
        String str = createString("hello world");
        ByteArray data = str->toByteArray();

        for(int i = 0;i < total;i++) {
            connection->publish("info",data,st(MqMessage)::PublishOneShot);
        }

        int result = 0;
        wait(&result);
        TEST_OK("testMqUnSubscribeMore case101");
    }

    return 0;
}

