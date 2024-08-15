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

CountDownLatch latch = CountDownLatch::New(1);
int total = 1024*12;

DECLARE_CLASS(CountHandler) IMPLEMENTS(Handler) {
public:
    void handleMessage(Message) {
        printf("total is %d \n",total);
        sendEmptyMessageDelayed(1,1000);
    }
};

DECLARE_CLASS(ConnectionListener) IMPLEMENTS(BroadcastConnectionListener) {
public:
    _ConnectionListener() {
        handler = createCountHandler();
        handler->sendEmptyMessageDelayed(1,1000);
    }

    int onMessage(String channel,ByteArray data) {
        String str = data->toString();
        if(str->equals("hello world")) {
            total--;
        }

        if(total == 0) {
            TEST_OK("testMqSubscribeUn case1");
            latch->countDown();
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
};

int main() {
    
    int port = getEnvPort();
    String url = String::New("tcp://127.0.0.1:")->append(String::New(port));

    int pid = fork();

    if(pid == 0) {
        sleep(1);

        BroadcastConnection connection = BroadcastConnection::New(url,ConnectionListener::New());
        connection->connect();
        int count = 0;
        while(count < 1024) {
            connection->subscribeChannel("info");
            connection->unSubscribeChannel("info");
            count++;
        }
        connection->subscribeChannel("info");
        latch->await();
    } else {
        DistributeCenterBuilder builder = DistributeCenterBuilder::New();
        builder->setUrl(url);
        BroadcastCenter center = builder->build();
        center->start();
        BroadcastConnection connection = BroadcastConnection::New(url);
        connection->connect();
        sleep(3);
        
        String str = String::New("hello world");
        ByteArray data = str->toByteArray();
        auto param = BroadcastMessage::NewParam();
        param->setFlags(st(BroadcastMessage)::OneShotFlag);

        for(int i = 0;i < total;i++) {
            connection->publishMessage("info",data,param);
        }

        int result = 0;
        wait(&result);

        port++;
        setEnvPort(port);
        TEST_OK("testMqSubscribeUn case100");
    }

    return 0;
}

