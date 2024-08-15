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
#include "QueueConnection.hpp"
#include "QueueCenter.hpp"
#include "FenceCenter.hpp"
#include "FenceConnection.hpp"
#include "TimeWatcher.hpp"

using namespace obotcha;
using namespace gagira;

int count = 0;

// DECLARE_CLASS(MyHandler) IMPLEMENTS(DistributeHandler) {
//     int onRequest(DistributeLinker,sp<_FenceMessage> msg) {
//         count++;
//         return 0;
//     }
// };

int main() {
    int port = getEnvPort();
    String url = String::New("tcp://127.0.0.1:")->append(String::New(port));
    
    // auto option = FenceOption::New();
    // option->setHandler(MyHandler::New());
    
    FenceCenter center = FenceCenter::New(url,nullptr);
    center->start();
    usleep(1000*100);
    
    FenceConnection c = FenceConnection::New(url);
    printf("main trace1 \n");
    int ret = c->connect();
    c->acquireFence(String::New("abc"));
    
    ret = c->releaseFence(String::New("abc"));
    if(ret != 0) {
        TEST_FAIL("testFence Double Release case1");
    }
    
    ret = c->releaseFence(String::New("abc"));
    if(ret != -EINVAL) {
        TEST_FAIL("testFence Double Release case2,ret is %d",ret);
    }
    
    setEnvPort(++port);
    TEST_OK("testFence Double Release case100");
    return 0;
}
