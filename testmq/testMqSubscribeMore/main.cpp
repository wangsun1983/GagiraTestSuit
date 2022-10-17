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
        MqConnection connection = createMqConnection("tcp://127.0.0.1:1260",createConnectionListener());
        connection->connect();
        int count = 0;
        while(count < 1024) {
            connection->subscribe("info");
            count++;
        }

        latch->await();
    } else {
        MqCenterBuilder builder = createMqCenterBuilder();
        builder->setUrl("tcp://127.0.0.1:1260");
        MqCenter center = builder->build();
        MqConnection connection = createMqConnection("tcp://127.0.0.1:1260");
        connection->connect();
        sleep(2);
        
        String str = createString("hello world");
        ByteArray data = str->toByteArray();

        for(int i = 0;i < total;i++) {
            connection->publish("info",data,st(MqMessage)::PublishOneShot);
        }

        int result = 0;
        wait(&result);
        TEST_OK("testMqSendLoop case100");
    }

    return 0;
}

