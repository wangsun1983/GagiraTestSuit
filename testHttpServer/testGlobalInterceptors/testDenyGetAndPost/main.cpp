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

using namespace obotcha;
using namespace gagira;

bool isEnableGet = false;

CountDownLatch latch = createCountDownLatch(1);

DECLARE_CLASS(SimpleController) IMPLEMENTS(Controller) {
public:
    HttpResponseEntity get() {
        
        TEST_FAIL("testGlobalInterceptors case1");
        
        return nullptr;
    }

    HttpResponseEntity post() {
        
        TEST_FAIL("testGlobalInterceptors case2");
        
        return nullptr;
    }

    HttpResponseEntity globalGet() {
        if(!isEnableGet){
            isEnableGet = true;
            return createHttpResponseEntity(st(HttpStatus)::NoContent);
        }

        return nullptr;
    }

    HttpResponseEntity globalPost() {
        return createHttpResponseEntity(st(HttpStatus)::ResetContent);
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
    InjectGlobalController(st(HttpMethod)::Post,getController,globalPost);

    InjectController(st(HttpMethod)::Get,"/simpleget",getController,get);
    InjectController(st(HttpMethod)::Post,"/simplepost",getController,post);
    InjectController(st(HttpMethod)::Get,"/complete",getController,complete);

    latch->await();
    server->close();
    
    port++;
    setEnvPort(port);
    TEST_OK("testGlobalInterceptors case100");

    return 0;
}
