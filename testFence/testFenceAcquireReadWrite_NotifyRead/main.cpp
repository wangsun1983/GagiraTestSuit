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

int main() {
    int port = getEnvPort();
    String url = String::New("tcp://127.0.0.1:")->append(String::New(port));

    FenceCenter center = FenceCenter::New(url,nullptr);
    center->start();
    usleep(1000*100);
    FenceConnection c = FenceConnection::New(url);
    c->connect();
    c->acquireWriteFence(String::New("abc"));
    auto watcher = TimeWatcher::New();
    watcher->start();
    int ret = c->acquireReadFence(String::New("abc"),200);
    auto cost = watcher->stop();
    if(ret != -ETIMEDOUT) {
        TEST_FAIL("testFenceAcquireWriteRead notify read case1,ret is %d",ret);
    }
    
    if(cost > 220) {
        TEST_FAIL("testFenceAcquireWriteRead notify read case2");
    }
    setEnvPort(++port);
    TEST_OK("testFenceAcquireWriteRead notify read case100");
    return 0;
}
