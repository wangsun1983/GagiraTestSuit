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
    
    auto pid = fork();
    if(pid == 0) {
        usleep(1000*100);
        FenceConnection c = FenceConnection::New(url);
        c->connect();
        auto ret = c->acquireFence(String::New("abc"));
        usleep(1000*250);
        c->releaseFence(String::New("abc"));
        c->close();
        exit(0);
    } else {
        auto option = FenceOption::New();
        FenceCenter center = FenceCenter::New(url,option);
        center->start();
        usleep(1000*150);
        FenceConnection c = FenceConnection::New(url);
        c->connect();
        
        FenceConnection c2 = FenceConnection::New(url);
        c2->connect();
        
        TimeWatcher watcher = TimeWatcher::New();
        watcher->start();
        auto ret = c->acquireFence(String::New("abc"));
        auto cost = watcher->stop();
        if(ret != 0) {
            TEST_FAIL("testFenceAcquire wait for other process exit case1,ret is %d",ret);
        }
        
        if(cost < 195 || cost > 300) {
            TEST_FAIL("testFenceAcquire wait for other process exit case2,cost is %d",cost);
        }
        
        Thread t1 = Thread::New([&c]{
            usleep(1000*100);
            c->releaseFence(String::New("abc"));
        });
        t1->start();
        
        
        watcher->start();
        ret = c2->acquireFence(String::New("abc"));
        cost = watcher->stop();
        
        if(cost < 95 || cost > 120) {
            TEST_FAIL("testFenceAcquire wait for other process exit case2,cost is %d",cost);
        }
        
        center->close();
    }
    setEnvPort(++port);
    TEST_OK("testFenceAcquire wait for other process exit case100");
    return 0;
}
