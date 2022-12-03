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
#include "NetPort.hpp"

using namespace obotcha;
using namespace gagira;

CountDownLatch latch1 = createCountDownLatch(1024*12);
CountDownLatch latch2 = createCountDownLatch(1024*12);

int total = 1024*12;

DECLARE_CLASS(CountHandler) IMPLEMENTS(Handler) {
public:
    void handleMessage(Message) {
        printf("latch1 is %d,latch2 is %d \n",latch1->getCount(),latch2->getCount());
        sendEmptyMessageDelayed(1,1000);
    }
};

DECLARE_CLASS(ConnectionListener) IMPLEMENTS(MqConnectionListener) {
public:
    _ConnectionListener() {
        handler = createCountHandler();
        handler->sendEmptyMessageDelayed(1,1000);
    }

    int onMessage(String channel,ByteArray data) {
        String str = data->toString();
        if(channel->equals("info") && str->equals("hello world")) {
            latch1->countDown();
        } else if(channel->equals("info2") && str->equals("hello world")) {
            latch2->countDown();
        }

        return 0;
    }

    void onDisconnect() {
        //return true;
    }
    
    void onConnect() {
        //return true;
    }

    void onDetach(String channel) {
        //return true;
    }

private:
    CountHandler handler;
};

int main() {
    
    int port = getEnvPort();
    String url = createString("tcp://127.0.0.1:")->append(createString(port));

    int pid = fork();

    if(pid == 0) {
        sleep(1);
        MqConnection connection = createMqConnection(url,createConnectionListener());
        connection->connect();
        connection->subscribeChannel("info");
        connection->subscribeChannel("info2");

        latch1->await();
        latch2->await();
    } else {
        MqCenterBuilder builder = createMqCenterBuilder();
        builder->setUrl(url);
        MqCenter center = builder->build();
        center->start();
        MqConnection connection = createMqConnection(url);
        connection->connect();
        sleep(3);
        
        String str = createString("hello world");
        ByteArray data = str->toByteArray();

        for(int i = 0;i < total;i++) {
            connection->publishMessage("info",data);
            connection->publishMessage("info2",data);
        }

        int result = 0;
        wait(&result);
        TEST_OK("testMqSubscribeUn case100");
        port++;
        setEnvPort(port);
    }

    return 0;
}

