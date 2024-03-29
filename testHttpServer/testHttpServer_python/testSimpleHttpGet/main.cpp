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
bool isGetHit = false;
bool isGet2Hit = false;

DECLARE_CLASS(SimpleGetController) IMPLEMENTS(Controller) {
public:
    HttpResponseEntity get() {
        String value = GetStringParam(value);
        if(!value->equals("100")) {
            TEST_FAIL("testSimpleHttpGet case1");
        }
        isGetHit = true;
        latch->countDown();
        return nullptr;
    }

    HttpResponseEntity get2() {
        String value = GetStringParam(value);
        if(!value->equals("200")) {
            TEST_FAIL("testSimpleHttpGet case2");
        }
        isGet2Hit = true;
        latch->countDown();
        return nullptr;
    }
};

int main() {
    int port = getEnvPort();
    Server server = createServer();
    server->setAddress(createInet4Address(port));
    server->start();
    SimpleGetController getController = createSimpleGetController();

    InjectController(st(HttpMethod)::Get,"/simpleget/{value}",getController,get);
    InjectController(st(HttpMethod)::Get,"/simpleget2/{value}",getController,get2);

    latch->await();
    server->close();
    
    if(!isGetHit || !isGet2Hit) {
        TEST_FAIL("testSimpleHttpGet case80");
    }

    port++;
    setEnvPort(port);
    TEST_OK("testSimpleHttpGet case100");

    return 0;
}
