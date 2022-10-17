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

DECLARE_CLASS(ConnectionListener) IMPLEMENTS(MqConnectionListener) {
public:
    bool onMessage(String channel,ByteArray data) {
        return true;
    }

    bool onDisconnect() {
      //TODO
      return false;
    }

    bool onConnect() {
      //TODO
      return false;
    }

    bool onDetach(String channel) {
      //TODO
      return false;
    }
};

int main() {

    int pid = fork();

    if(pid != 0) {
        sleep(1);
        MqConnection connection = createMqConnection("tcp://127.0.0.1:1280",createConnectionListener());
        connection->connect();
        connection->unSubscribe("info");
        TEST_OK("testMqUnsubscribeWithNoSubscribe case100");
    } else {
        MqCenterBuilder builder = createMqCenterBuilder();
        builder->setUrl("tcp://127.0.0.1:1280");
        MqCenter center = builder->build();
        sleep(5);
    }

    return 0;
}
