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
#include "ArchiveConnection.hpp"
#include "ArchiveCenter.hpp"
#include "TestLog.hpp"

using namespace obotcha;
using namespace gagira;

int main() {
    int port = getEnvPort();
    String url = String::New("tcp://127.0.0.1:")->append(String::New(port));
    
    ArchiveCenter center = ArchiveCenter::New(url,ArchiveOption::New());
    center->start();
    usleep(1000*100);
    ArchiveConnection c = ArchiveConnection::New(url);
    c->connect();
    int ret = c->download("testdata","./tmp/abc");
    if(ret != -ENOENT) {
        TEST_FAIL("testDocuement NotFound case1,ret is %d",ret);
    }
    
    sleep(1);

    setEnvPort(++port);
    TEST_OK("testDocuement NotFound case100");
    return 0;
}
