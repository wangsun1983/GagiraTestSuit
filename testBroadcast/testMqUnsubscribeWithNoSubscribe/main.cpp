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

DECLARE_CLASS(ConnectionListener) IMPLEMENTS(MqConnectionListener) {
public:
    int onMessage(String channel,ByteArray data) {
        return 0;
    }

    void onDisconnect() {
      //TODO
      //return false;
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

    int pid = fork();
    int port = getEnvPort();
    String url = createString("tcp://127.0.0.1:")->append(createString(port));

    if(pid != 0) {
        sleep(1);
        MqConnection connection = createMqConnection(url,createConnectionListener());
        connection->connect();
        connection->unSubscribeChannel("info");
        TEST_OK("testMqUnsubscribeWithNoSubscribe case100");
    } else {
        MqCenterBuilder builder = createMqCenterBuilder();
        builder->setUrl(url);
        MqCenter center = builder->build();
        center->start();
        port++;
        setEnvPort(port);
        sleep(5);
    }

    return 0;
}
