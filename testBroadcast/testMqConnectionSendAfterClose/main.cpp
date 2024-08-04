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
        return 0;
    }

    void onDisconnect() {
      printf("disconnect \n");
      latch->countDown();
    }

    void onConnect() {
      //TODO
      //return false;
    }

    void onDetach(String channel) {
      //TODO
      //return false;
    }

    void onSustain(int code,String msg){}
};

int main() {

    int port = getEnvPort();
    String url = String::New("tcp://127.0.0.1:")->append(String::New(1284));

    int pid = fork();

    if(pid != 0) {
        sleep(1);
        MqConnection connection = createMqConnection(url,createConnectionListener());
        connection->connect();
        connection->subscribeChannel("info");
        latch->await();
        bool result = connection->publishMessage(String::New("info"),String::New("hello"));
        if(result) {
            TEST_FAIL("testmqsendAfterClose case100");
        }

        setEnvPort(++port);
        TEST_OK("testmqsendAfterClose case100");
    } else {
        MqCenterBuilder builder = createMqCenterBuilder();
        builder->setUrl(url);
        MqCenter center = builder->build();
        int ret = center->start();
        sleep(2);
        center->close();
    }

    return 0;
}
