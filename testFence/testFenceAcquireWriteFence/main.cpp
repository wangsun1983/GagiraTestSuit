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

    Thread t1 = createThread([&]{
        usleep(1000*100);
        FenceConnection c = createFenceConnection(url);
        c->connect();
        TimeWatcher watch = createTimeWatcher();
        watch->start();
        c->acquireReadFence(createString("abc"));
        auto cost = watch->stop();
        if(cost > 50) {
          TEST_FAIL("testFenceAcquireWriteFnece case1 ,cost is %d",cost);
        }

        sleep(3);
        c->releaseReadFence(createString("abc"));
        printf("t3 start \n");
    });

    Thread t2 = createThread([&]{
        usleep(1000 * 1000);
        FenceConnection c = createFenceConnection(url);
        c->connect();

        TimeWatcher watch = createTimeWatcher();
        watch->start();
        c->acquireWriteFence(createString("abc"));
        auto cost = watch->stop();
        if(cost < 2100 || cost > 2150) {
          TEST_FAIL("testFenceAcquireWriteFnece case2 ,cost is %d",cost);
        }
        c->releaseWriteFence(createString("abc"));
    });

    Thread t3 = createThread([&]{
        FenceConnection c = createFenceConnection(url);
        c->connect();
        TimeWatcher watch = createTimeWatcher();
        c->acquireReadFence(createString("abc"));
        sleep(3);
        c->releaseReadFence(createString("abc"));
    });

    t1->start();
    t2->start();
    t3->start();

    t1->join();
    t2->join();
    t3->join();

    setEnvPort(++port);
    TEST_OK("testFenceAcquireWriteFnece case100");
    return 0;
}
