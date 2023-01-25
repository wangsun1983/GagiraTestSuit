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

CountDownLatch latch = createCountDownLatch(1);
bool isGetHit = false;
bool isGet2Hit = false;
AtomicInteger mValue = createAtomicInteger(0);

DECLARE_CLASS(SimpleGetController) IMPLEMENTS(Controller) {
public:
    HttpResponseEntity get() {
        String value = GetStringParam(value);
        mValue->addAndGet(value->toInteger()->toValue());
        return nullptr;
    }

    HttpResponseEntity complete() {
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
    InjectController(st(HttpMethod)::Get,"/complete/",getController,complete);

    latch->await();
    server->close();
    
    int sum = 0;
    for(int i = 0; i < 32;i++) {
        sum += i;
    }

    sum = sum * 32;
    if(sum != mValue->get()) {
        TEST_FAIL("testSimpleHttpGet thread case1,mValue is %d,sum is %d",mValue->get(),sum);    
    }

    port++;
    setEnvPort(port);
    TEST_OK("testSimpleHttpGet thread case100");

    return 0;
}
