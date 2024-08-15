#include <stdio.h>
#include <unistd.h>

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

DECLARE_CLASS(ConnectionListener) IMPLEMENTS(BroadcastConnectionListener) {
public:
    int onMessage(String channel,ByteArray data) {
        printf("onMessage \n");
        for(int i = 0;i < 1024;i++) {
            if(data[i] != i%255) {
                TEST_FAIL("testMqSendByteArray case1,data[%d] is %d",i,data[i]);
            }
        }
        TEST_OK("testMqSendByteArray case2");
        latch->countDown();
        return 0;
    }

    void onDisconnect(){}
    void onConnect(){}
    void onDetach(String channel){}
    void onSustain(int code,String msg){}
};

int main() {
    
    int port = getEnvPort();
    String url = String::New("tcp://127.0.0.1:")->append(String::New(port));
    int pid = fork();

    if(pid != 0) {
        sleep(1);
        printf("connection trace1 \n");
        BroadcastConnection connection = BroadcastConnection::New(url,ConnectionListener::New());
        connection->connect();
        printf("connection trace2 \n");
        connection->subscribeChannel("info");
        printf("connection trace3 \n");
        latch->await();
    } else {
        DistributeCenterBuilder builder = DistributeCenterBuilder::New();
        builder->setUrl(url);
        BroadcastCenter center = builder->build();
        center->start();
        usleep(1000*100);
        BroadcastConnection connection = BroadcastConnection::New(url);
        connection->connect();
        
        ByteArray array = ByteArray::New(1024);
        for(int i = 0;i<1024;i++) {
            array[i] = i%255;
        }

        BroadcastMessageParam param = BroadcastMessage::NewParam();
        param->setFlags(st(BroadcastMessage)::OneShotFlag|st(BroadcastMessage)::AcknowledgeFlag);
        connection->publishMessage("info",
                                    array,
                                    param);

        sleep(5);
        port++;
        setEnvPort(port);
    }

    return 0;
}

