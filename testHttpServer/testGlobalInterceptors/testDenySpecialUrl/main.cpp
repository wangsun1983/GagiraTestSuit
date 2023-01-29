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
#include "Reflect.hpp"
#include "Utils.hpp"

using namespace obotcha;
using namespace gagira;

bool isEnableGet = false;

CountDownLatch latch = createCountDownLatch(1);

DECLARE_CLASS(SimpleController) IMPLEMENTS(Controller) {
public:
    HttpResponseEntity get() {
        
        TEST_FAIL("testDenySpecialUrl case1");
        
        return nullptr;
    }

    HttpResponseEntity okget() {
        printf("abc \n");
        return createHttpResponseEntity(st(HttpStatus)::Ok);
    }


    
    HttpResponseEntity globalGet() {
        auto p = GetPacket();
        HttpUrl url = p->getHeader()->getUrl();
        String path = url->getPath();
        printf("path is %s \n",url->getPath()->toChars());
        
        if(url->getPath()->equals("okget")||url->getPath()->equals("complete")) {
            return nullptr;
        }

        return createHttpResponseEntity(st(HttpStatus)::NoContent);
    }

    HttpResponseEntity complete() {
        latch->countDown();
        return nullptr;
    }
};

int main() {
    int port = getEnvPort();
    Server server = createServer();
    port = 3006;
    server->setAddress(createInet4Address(port));
    server->start();
    SimpleController getController = createSimpleController();
    InjectGlobalController(st(HttpMethod)::Get,getController,globalGet);

    InjectController(st(HttpMethod)::Get,"/simpleget",getController,get);
    InjectController(st(HttpMethod)::Post,"/okget",getController,okget);
    InjectController(st(HttpMethod)::Get,"/complete",getController,complete);

    latch->await();
    server->close();
    
    port++;
    setEnvPort(port);
    TEST_OK("testDenySpecialUrl case100");

    return 0;
}
