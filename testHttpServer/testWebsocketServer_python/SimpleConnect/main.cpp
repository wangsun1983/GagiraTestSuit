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
#include "MqSustainMessage.hpp"

using namespace obotcha;
using namespace gagira;

CountDownLatch latch = createCountDownLatch(2);

DECLARE_CLASS(SimpleGetController) IMPLEMENTS(Controller) {
public:
    WsResponseEntity get() {
        String value = GetWebsocketRequest()->toString();
        if(!value->equals("Hello, World")) {
            TEST_FAIL("testSimpleHttpGet case1");
        }
        latch->countDown();
        return nullptr;
    }

    WsResponseEntity complete() {
        latch->countDown();
        return nullptr;
    }
};

int main() {
    int port = getEnvPort();
    Server server = createServer();
    //server->setAddress(createInet4Address(port));
    server->setWsAddress(createInet4Address(port));
    
    SimpleGetController getController = createSimpleGetController();
    
    server->start();
    InjectWsController("simpleget",getController,get);
    InjectWsController("complete",getController,complete);


    latch->await();
    server->close();
    
    port++;
    setEnvPort(port);
    TEST_OK("testSimpleHttpGet case100");

    return 0;
}
