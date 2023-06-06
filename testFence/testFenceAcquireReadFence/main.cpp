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

    FenceCenter center = createFenceCenter(url,nullptr);
    center->start();
    usleep(1000*100);

    Thread t1 = createThread([&]{
        FenceConnection c = createFenceConnection(url);
        c->connect();
        c->acquireReadFence(createString("abc"));
        sleep(5);
        c->releaseReadFence(createString("abc"));
    });

    Thread t2 = createThread([&]{
        usleep(1000 * 100);
        FenceConnection c = createFenceConnection(url);
        c->connect();

        TimeWatcher watch = createTimeWatcher();
        watch->start();
        c->acquireWriteFence(createString("abc"));
        auto cost = watch->stop();
        if(cost < 4800 || cost > 5050) {
          TEST_FAIL("testFenceAcquireReadFence case1 ,cost is %d \n",cost);
        }
        printf("cost is %d \n",cost);
        c->releaseWriteFence(createString("abc"));
    });

    Thread t3 = createThread([&]{
        usleep(1000*100);
        FenceConnection c = createFenceConnection(url);
        c->connect();

        TimeWatcher watch = createTimeWatcher();
        watch->start();
        c->acquireReadFence(createString("abc"));
        auto cost = watch->stop();
        if(cost > 50) {
          TEST_FAIL("testFenceAcquireReadFence case2 ,cost is %d",cost);
        }
        usleep(1000*5000);
        c->releaseReadFence(createString("abc"));
    });

    t1->start();
    t2->start();
    t3->start();

    t1->join();
    t2->join();
    t3->join();

    setEnvPort(++port);
    TEST_OK("testFenceAcquireReadFence case100");
    return 0;
}
