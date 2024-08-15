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

    Thread t1 = Thread::New([&]{
        FenceConnection c = FenceConnection::New(url);
        c->connect();
        c->acquireWriteFence(String::New("abc"));
        usleep(1000*500);
        c->releaseWriteFence(String::New("abc"));
    });

    Thread t2 = Thread::New([&]{
        usleep(1000 * 100);
        FenceConnection c = FenceConnection::New(url);
        c->connect();
        auto watcher = TimeWatcher::New();
        watcher->start();
        c->acquireReadFence(String::New("abc"));
        auto cost = watcher->stop();
        if(cost < 400 || cost > 440) {
            TEST_FAIL("testFenceAcquireReadFence After Write release case1,cost is %d",cost);
        }
        usleep(1000*400);
    });
    
    Thread t3 = Thread::New([&]{
        usleep(1000 * 100);
        FenceConnection c = FenceConnection::New(url);
        c->connect();
        auto watcher = TimeWatcher::New();
        watcher->start();
        c->acquireReadFence(String::New("abc"));
        auto cost = watcher->stop();
        if(cost < 400 || cost > 450) {
            TEST_FAIL("testFenceAcquireReadFence After Write release case2,cost is %d",cost);
        }
        usleep(1000*400);
    });
    
    t1->start();
    t2->start();
    t3->start();

    t1->join();
    t2->join();
    t3->join();

    setEnvPort(++port);
    TEST_OK("testFenceAcquireReadFence After Write release case100");
    return 0;
}
