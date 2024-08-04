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

    printf("trace1 \n");
    FenceCenter center = createFenceCenter(url,nullptr);
    center->start();
    usleep(1000*100);

    Thread t1 = Thread::New([&]{
        FenceConnection c = createFenceConnection(url);
        c->connect();
        c->acquireReadFence(String::New("abc"));
        sleep(2);
        c->close();
    });

    Thread t2 = Thread::New([&]{
        usleep(1000*100);
        FenceConnection c = createFenceConnection(url);
        c->connect();

        auto watch = createTimeWatcher();
        watch->start();
        c->acquireReadFence(String::New("abc"));
        auto cost = watch->stop();
        if(cost > 15) {
          TEST_FAIL("testReadFenceDisocnnect2 case1,cost is %d",cost);
        }
        sleep(4);
        c->close();
    });

    Thread t3 = Thread::New([&]{
        usleep(1000 * 3000);
        FenceConnection c = createFenceConnection(url);
        c->connect();

        TimeWatcher watch = createTimeWatcher();
        watch->start();
        c->acquireWriteFence(String::New("abc"));
        auto cost = watch->stop();
        if(cost > 1150 || cost < 1100) {
          TEST_FAIL("testReadFenceDisocnnect2 case2,cost is %d",cost);
        }
        c->releaseWriteFence(String::New("abc"));
    });

    t1->start();
    t2->start();
    t3->start();

    t1->join();
    t2->join();
    t3->join();

    setEnvPort(++port);
    TEST_OK("testReadFenceDisocnnect2 case100");
    return 0;
}
