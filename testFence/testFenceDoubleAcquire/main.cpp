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

        TimeWatcher watch = TimeWatcher::New();
        c->acquireFence(String::New("abc"));
        c->acquireFence(String::New("abc"));
        c->releaseFence(String::New("abc"));
        usleep(1000*1000);
        c->releaseFence(String::New("abc"));
    });

    Thread t2 = Thread::New([&]{
        usleep(1000*100);
        FenceConnection c = FenceConnection::New(url);
        c->connect();

        TimeWatcher watch = TimeWatcher::New();
        watch->start();
        c->acquireFence(String::New("abc"));
        auto cost = watch->stop();
        if(cost < 900 || cost > 980) {
          TEST_FAIL("testFenceDoubleAcquire case1 ,cost is %d",cost);
        }
        c->releaseWriteFence(String::New("abc"));
    });


    t1->start();
    t2->start();
    t1->join();
    t2->join();

    setEnvPort(++port);
    TEST_OK("testFenceDoubleAcquire case100");
    return 0;
}
