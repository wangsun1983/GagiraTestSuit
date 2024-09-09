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
#include "FenceMutex.hpp"
#include "FenceFactory.hpp"

using namespace obotcha;
using namespace gagira;

int main() {
    int port = getEnvPort();
    String url = String::New("tcp://127.0.0.1:")->append(String::New(port));

    FenceCenter center = FenceCenter::New(url,nullptr);
    center->start();
    usleep(1000*100);
    
    Thread t1 = Thread::New([url]{
        auto factory = FenceFactory::New(url);
        auto rwlock = factory->getFenceReadWriteLock(String::New("abc"));
        auto watcher = TimeWatcher::New();
        watcher->start();
        AutoLock l(rwlock->getWriteLock());
        auto cost = watcher->stop();
        if(cost > 20) {
            TEST_FAIL("testFenceAcquire FenceReadWriteLock MultiWrite case1,cost is %d",cost);
        }
        usleep(1000*500);
    });
    
    Thread t2 = Thread::New([url]{
        auto watcher = TimeWatcher::New();
        watcher->start();
        usleep(1000*100);
        auto factory = FenceFactory::New(url);
        auto rwlock = factory->getFenceReadWriteLock(String::New("abc"));
        AutoLock l(rwlock->getWriteLock());
        auto cost = watcher->stop();
        if(cost < 500 || cost > 530) {
            TEST_FAIL("testFenceAcquire FenceReadWriteLock MultiWrite case2,cost is %d",cost);
        }
        usleep(1000*500);
    });
    
    Thread t3 = Thread::New([url]{
        auto watcher = TimeWatcher::New();
        watcher->start();
        usleep(1000*200);
        auto factory = FenceFactory::New(url);
        auto rwlock = factory->getFenceReadWriteLock(String::New("abc"));
        
        AutoLock l(rwlock->getReadLock());
        auto cost = watcher->stop();
        if(cost > 10020) {
            TEST_FAIL("testFenceAcquire FenceReadWriteLock MultiWrite case3,cost is %d",cost);
        }
        usleep(1000*500);
    });
    
    t1->start();
    t2->start();
    t3->start();
    
    t1->join();
    t2->join();
    t3->join();
    
    setEnvPort(++port);
    TEST_OK("testFenceAcquire FenceReadWriteLock MultiWrite case100");
    return 0;
}
