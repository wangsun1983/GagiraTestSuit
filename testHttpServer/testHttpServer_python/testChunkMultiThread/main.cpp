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
#include "SampleFile.hpp"
#include "ForEveryOne.hpp"
#include "Md.hpp"

using namespace obotcha;
using namespace gagira;

CountDownLatch latch = createCountDownLatch(1);

DECLARE_CLASS(SimplePostController) IMPLEMENTS(Controller) {
public:
    HttpResponseEntity get() {
        return createHttpResponseEntity(createHttpChunk(createFile("./tmp/testdata")));
    }

    HttpResponseEntity complete() {
        latch->countDown();
        return nullptr;
    }

};

int main() {
    //create test data
    createSampleFile(createFile("./tmp/testdata"),1024*1024*2);

    int port = getEnvPort();
    Server server = createServer();
    port = getEnvPort();
    server->setAddress(createInet4Address(port))
          ->setMultiPartFilePath("./tmp/");

    server->start();
    SimplePostController postController = createSimplePostController();

    InjectController(st(HttpMethod)::Get,"/simpleget",postController,get);
    InjectController(st(HttpMethod)::Get,"/completeget",postController,complete);
    latch->await();
    server->close();
    port++;
    setEnvPort(port);

    for(int i = 0;i < 8*32;i++) {
        File f = createFile(createString("./tmp/")->append(createString(i)));
        Md md5 = createMd();
        //printf("check file id is %d \n",i);
        String v1 = md5->encrypt(createFile("./tmp/testdata"));
        String v2 = md5->encrypt(f);
        if(v1 != v2) {
          TEST_FAIL("testHttpChunk MultiThread case1 error,path is %s",f->getAbsolutePath()->toChars());
        }
    }
    TEST_OK("testHttpChunk MultiThread case100");

    return 0;
}
