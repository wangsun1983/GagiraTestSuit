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

CountDownLatch latch = createCountDownLatch(32);

DECLARE_CLASS(SimplePostController) IMPLEMENTS(Controller) {
public:
    HttpResponseEntity get() {
        auto req = getRequest();
        auto multiPart = req->getMultiPart();
        if(multiPart != nullptr && multiPart->getFiles() != nullptr) {
          auto files = multiPart->getFiles();
          ForEveryOne(file,files) {
            File f = file->getFile();
            //start md5 check
            Md md5 = createMd();
            String v1 = md5->encrypt(createFile("./tmp/testdata"));
            String v2 = md5->encrypt(createFile(f->getAbsolutePath()->toChars()));
            if(v1 != v2) {
              TEST_FAIL("testMutltiPart case1 error,path is %s",f->getAbsolutePath()->toChars());
            }
          }
        }
        latch->countDown();
        return nullptr;
    }

};

int main() {
    //create test data
    createSampleFile(createFile("./tmp/testdata"),1024*1024*16);

    int port = getEnvPort();
    Server server = createServer();
    port = getEnvPort();
    server->setAddress(createInet4Address(port))
          ->setMultiPartFilePath("./tmp/");

    server->start();
    SimplePostController postController = createSimplePostController();

    InjectController(st(HttpMethod)::Post,"/simplepost/{value}",postController,get);
    latch->await();
    server->close();
    port++;
    setEnvPort(port);
    TEST_OK("testMutltiPart case100");

    return 0;
}
