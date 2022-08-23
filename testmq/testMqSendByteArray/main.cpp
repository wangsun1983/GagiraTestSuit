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

using namespace obotcha;
using namespace gagira;

CountDownLatch latch = createCountDownLatch(1);

DECLARE_CLASS(ConnectionListener) IMPLEMENTS(MqConnectionListener) {
public:
    bool onEvent(String channel,ByteArray data) {
        if(data == nullptr) {
            printf("data is nullptr \n");
        } else {
            printf("data is not nullptr,size is %d \n",data->size());
        }
        for(int i = 0;i < 1024;i++) {
            if(data[i] != i%255) {
                TEST_FAIL("testMqSendByteArray case1,data[%d] is %d",i,data[i]);
            }
        }
        TEST_OK("testMqSendByteArray case2");
        latch->countDown();
        return true;
    }
};

int main() {
    
    int pid = fork();

    if(pid != 0) {
        sleep(1);
        MqConnection connection = createMqConnection("tcp://127.0.0.1:1260");
        connection->connect();
        connection->subscribe("info",createConnectionListener());
        latch->await();
    } else {
        MqCenterBuilder builder = createMqCenterBuilder();
        builder->setUrl("tcp://127.0.0.1:1260");
        MqCenter center = builder->build();
        MqConnection connection = createMqConnection("tcp://127.0.0.1:1260");
        connection->connect();
        
        ByteArray array = createByteArray(1024);
        for(int i = 0;i<1024;i++) {
            array[i] = i%255;
        }

        connection->publish("info",array,st(MqMessage)::PublishOneShot);

        sleep(5);
    }

    return 0;
}

