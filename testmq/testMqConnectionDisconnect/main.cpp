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

int gDisconnect = 0;
int gConnect = 0;

DECLARE_CLASS(ConnectionListener) IMPLEMENTS(MqConnectionListener) {
public:
    bool onMessage(String channel,ByteArray data) {
        return true;
    }

    bool onDisconnect() {
      gDisconnect = 100;
      return false;
    }

    bool onConnect() {
      gConnect = 200;
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
        sleep(10);
        if(gDisconnect != 100) {
          TEST_FAIL("testMqConnectionDisconnect case1,disconnect is %d",gDisconnect);
        }

        if(gConnect != 200) {
          TEST_FAIL("testMqConnectionConnect case2,connect is %d",gConnect);
        }

        TEST_OK("testMqConnectionDisconnect case3");
    } else {
        MqCenterBuilder builder = createMqCenterBuilder();
        builder->setUrl("tcp://127.0.0.1:1280");
        MqCenter center = builder->build();
        sleep(5);
        center->close();
        sleep(1);
    }

    return 0;
}
