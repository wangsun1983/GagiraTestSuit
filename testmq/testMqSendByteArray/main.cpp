#include <stdio.h>
#include <unistd.h>

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

CountDownLatch latch = createCountDownLatch(1);

DECLARE_CLASS(ConnectionListener) IMPLEMENTS(MqConnectionListener) {
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
};

int main() {
    
    int port = getEnvPort();
    String url = createString("tcp://127.0.0.1:")->append(createString(port));
    int pid = fork();

    if(pid != 0) {
        sleep(1);
        printf("connection trace1 \n");
        MqConnection connection = createMqConnection(url,createConnectionListener());
        connection->connect();
        printf("connection trace2 \n");
        connection->subscribeChannel("info");
        printf("connection trace3 \n");
        latch->await();
    } else {
        MqCenterBuilder builder = createMqCenterBuilder();
        builder->setUrl(url);
        MqCenter center = builder->build();
        center->start();
        usleep(1000*100);
        MqConnection connection = createMqConnection(url);
        connection->connect();
        
        ByteArray array = createByteArray(1024);
        for(int i = 0;i<1024;i++) {
            array[i] = i%255;
        }

        connection->publishMessage("info",
                                    array,
                                    st(MqMessage)::OneShotFlag|st(MqMessage)::AcknowledgeFlag);

        sleep(5);
        port++;
        setEnvPort(port);
    }

    return 0;
}

