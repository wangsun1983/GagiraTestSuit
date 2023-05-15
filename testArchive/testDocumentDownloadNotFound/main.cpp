#include <stdio.h>
#include <unistd.h>

#include "Controller.hpp"
#include "Server.hpp"
#include "Inet4Address.hpp"
#include "ServletRequest.hpp"
#include "HttpResourceManager.hpp"
#include "Reflect.hpp"
#include "Utils.hpp"
#include "Serializable.hpp"
#include "CountDownLatch.hpp"
#include "TestLog.hpp"
#include "Handler.hpp"
#include "NetPort.hpp"
#include "Thread.hpp"
#include "DocumentConnection.hpp"
#include "DocumentCenter.hpp"

using namespace obotcha;
using namespace gagira;

int main() {
    int port = getEnvPort();
    String url = createString("tcp://127.0.0.1:")->append(createString(port));
    
    DocumentCenter center = createDocumentCenter(url,createDocumentOption());
    center->start();
    usleep(1000*100);
    DocumentConnection c = createDocumentConnection(url);
    c->connect();
    int ret = c->download("testdata","./tmp/abc");
    if(ret != -ENOENT) {
        printf("ret is not noent,ret is %d \n",ret);
    }
    
    printf("finish \n");
    sleep(1);

    setEnvPort(++port);
    sleep(1);
    return 0;
}
