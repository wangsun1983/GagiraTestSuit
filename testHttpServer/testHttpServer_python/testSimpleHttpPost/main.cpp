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

CountDownLatch latch = createCountDownLatch(2);
bool isGetHit = false;
bool isGet2Hit = false;

DECLARE_CLASS(SampleData) {
public:
    int a;
    String b;
    DECLARE_REFLECT_FIELD(SampleData,a,b);
};

DECLARE_CLASS(SimpleGetController) IMPLEMENTS(Controller) {
public:
    HttpResponseEntity get() {
        String value = GetStringParam(value);
        if(!value->equals("100")) {
            TEST_FAIL("testSimpleHttpPost case1");
        }
        isGetHit = true;
        latch->countDown();

        SampleData data = createSampleData();
        data->a = 100;
        data->b = createString("hello,this is server");
        return createHttpResponseEntity(data);
    }

    HttpResponseEntity get2() {
        ServletRequest req = getRequest();
        SampleData info = req->getContent<SampleData>();
        if(info->a != 100 || !info->b->equals("hello,this is server")) {
            TEST_FAIL("testSimpleHttpPost case2");
        }
        isGet2Hit = true;
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
    SimpleGetController getController = createSimpleGetController();

    InjectController(st(HttpMethod)::Get,"/simpleget/{value}",getController,get);
    InjectController(st(HttpMethod)::Post,"/simplepost2/{value}",getController,get2);

    latch->await();
    server->close();
    
    if(!isGetHit || !isGet2Hit) {
        TEST_FAIL("testSimpleHttpPost case80");
    }

    port++;
    setEnvPort(port);
    TEST_OK("testSimpleHttpPost case100");

    return 0;
}
