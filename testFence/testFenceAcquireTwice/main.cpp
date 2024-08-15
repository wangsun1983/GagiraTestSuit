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
#include "TestLog.hpp"

using namespace obotcha;
using namespace gagira;

int main() {
    int port = getEnvPort();
    String url = String::New("tcp://127.0.0.1:")->append(String::New(port));

    FenceCenter center = FenceCenter::New(url,nullptr);
    center->start();
    usleep(1000*100);
    FenceConnection c1 = FenceConnection::New(url);
    c1->connect();
    int ret = c1->acquireFence(String::New("abc"));
    if(ret != 0) {
        TEST_FAIL("testFenceAcquire Acquire twice case1,ret is %d",ret);
    }
    
    auto watcher = TimeWatcher::New();
    watcher->start();
    ret = c1->acquireFence(String::New("abc"));
    auto cost = watcher->stop();
    if(ret != 0) {
        TEST_FAIL("testFenceAcquire Acquire twice case2,ret is %d",ret);
    }
    
    if(cost >20) {
        TEST_FAIL("testFenceAcquire Acquire twice case3,cost is %d",cost);
    }
    
    FenceConnection c2 = FenceConnection::New(url);
    c2->connect();
    
    Thread t1 = Thread::New([&c1]{
        usleep(1000*500);
        c1->releaseFence(String::New("abc"));
    });
    
    Thread t2 = Thread::New([&c1]{
        usleep(1000*1000);
        c1->releaseFence(String::New("abc"));
    });
    t1->start();
    t2->start();
    
    watcher->start();
    ret = c2->acquireFence(String::New("abc"));
    cost = watcher->stop();
    
    if(cost < 950 || cost > 1100) {
        TEST_FAIL("testFenceAcquire Acquire twice case4,cost is %d",cost);
    }

    setEnvPort(++port);
    TEST_OK("testFenceAcquire Acquire twice case100");
    return 0;
}
