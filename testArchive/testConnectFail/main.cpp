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

using namespace obotcha;
using namespace gagira;

int main() {
    int port = getEnvPort();
    String url = String::New("tcp://127.0.0.1:")->append(String::New(port));
    
    ArchiveConnection c = ArchiveConnection::New(url);
    int ret = c->connect();
    if(ret != -ENETUNREACH) {
        TEST_FAIL("testDocuement connect case1,ret is %d",ret);
    }
    
    sleep(1);
    setEnvPort(++port);
    TEST_OK("testDocuement connect case100");
    return 0;
}
