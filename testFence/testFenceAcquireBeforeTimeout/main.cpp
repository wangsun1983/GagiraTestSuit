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

    Thread t1 = Thread::New([&]{
        FenceConnection c = FenceConnection::New(url);
        c->connect();
        c->acquireFence(String::New("abc"));
        usleep(1000*100);
        c->releaseFence(String::New("abc"));
    });

    Thread t2 = Thread::New([&]{
        usleep(1000 * 50);
        FenceConnection c = FenceConnection::New(url);
        c->connect();
        auto watcher = TimeWatcher::New();
        watcher->start();
        c->acquireFence(String::New("abc"),300);
        auto cost = watcher->stop();
        if(cost > 100) {
            TEST_FAIL("testFenceAcquire Before Timeout case1,cost is %d",cost);
        }
        usleep(1000*500);
        int ret = c->acquireFence(String::New("abc"));
        if(ret != 0) {
            TEST_FAIL("testFenceAcquire Before Timeout case2,ret is %d",ret);
        }
    });

    t1->start();
    t2->start();

    t1->join();
    t2->join();

    setEnvPort(++port);
    TEST_OK("testFenceAcquire Before Timeout case100");
    return 0;
}
