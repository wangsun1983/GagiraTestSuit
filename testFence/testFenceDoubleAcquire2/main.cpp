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
    FenceConnection c = createFenceConnection(url);
    c->connect();

    TimeWatcher watch = createTimeWatcher();

    Thread t1 = Thread::New([&]{
        TimeWatcher watch = createTimeWatcher();
        printf("t1 start \n");
        c->acquireFence(String::New("abc"));
        c->acquireFence(String::New("abc"));
        c->releaseWriteFence(String::New("abc"));
        usleep(1000*1000);
        c->releaseWriteFence(String::New("abc"));
        printf("t2 trace \n");
    });

    Thread t2 = Thread::New([&]{
        usleep(1000*100);
        FenceConnection c = createFenceConnection(url);
        c->connect();
        c->acquireFence(String::New("abc"));
        auto cost = watch->stop();
        if(cost > 50) {
          TEST_FAIL("testFenceDoubleAcquire2 case1 cost is %d",cost);
        }
        c->releaseWriteFence(String::New("abc"));
    });


    t1->start();
    t2->start();

    t1->join();

    watch->start();
    c->close();
    t2->join();
    setEnvPort(++port);

    TEST_OK("testFenceDoubleAcquire2 case100");
    return 0;
}
