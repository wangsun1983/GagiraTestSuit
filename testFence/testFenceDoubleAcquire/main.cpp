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

        TimeWatcher watch = createTimeWatcher();
        c->acquireFence(createString("abc"));
        c->acquireFence(createString("abc"));
        c->releaseWriteFence(createString("abc"));
        usleep(1000*1000);
        c->releaseWriteFence(createString("abc"));
    });

    Thread t2 = createThread([&]{
        usleep(1000*100);
        FenceConnection c = createFenceConnection(url);
        c->connect();

        TimeWatcher watch = createTimeWatcher();
        watch->start();
        c->acquireFence(createString("abc"));
        auto cost = watch->stop();
        if(cost < 900 || cost > 950) {
          TEST_FAIL("testFenceDoubleAcquire case1 ,cost is %d",cost);
        }
        c->releaseWriteFence(createString("abc"));
        printf("t2 trace3 \n");
    });


    t1->start();
    t2->start();
    t1->join();
    t2->join();

    setEnvPort(++port);
    TEST_OK("testFenceDoubleAcquire case100");
    return 0;
}
