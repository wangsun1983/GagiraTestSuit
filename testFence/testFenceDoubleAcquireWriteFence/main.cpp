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
    String url = createString("tcp://127.0.0.1:")->append(createString(port));

    printf("trace1 \n");
    FenceCenter center = createFenceCenter(url,nullptr);
    center->start();
    usleep(1000*100);
    TimeWatcher watch = createTimeWatcher();

    Thread t1 = createThread([&]{
        usleep(1000*200);
        FenceConnection c = createFenceConnection(url);
        c->connect();

        TimeWatcher local_watch = createTimeWatcher();
        local_watch->start();
        c->acquireReadFence(createString("abc"));
        auto cost = watch->stop();
        if(cost > 50) {
          TEST_FAIL("testFenceDoubleAcquireWriteFence case1,cost is %d",cost);
        }

        auto local_cost = local_watch->stop();
        if(local_cost > 1860) {
          TEST_FAIL("testFenceDoubleAcquireWriteFence case2,cost is %d",local_cost);
        }
        
        watch->start();
        c->releaseReadFence(createString("abc"));
        cost = watch->stop();
        if(cost > 15) {
          TEST_FAIL("testFenceDoubleAcquireWriteFence case3,cost is %d",cost);
        }
    });

    Thread t2 = createThread([&]{
        FenceConnection c = createFenceConnection(url);
        c->connect();

        c->acquireWriteFence(createString("abc"));
        watch->start();
        c->acquireWriteFence(createString("abc"));
        c->releaseWriteFence(createString("abc"));

        usleep(1000*2000);
        watch->start();
        c->releaseWriteFence(createString("abc"));
        usleep(1000*3000);
    });

    t1->start();
    t2->start();

    t1->join();
    t2->join();

    setEnvPort(++port);
    TEST_OK("testFenceDoubleAcquireWriteFence case100");
    return 0;
}
