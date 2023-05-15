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
        FenceConnection c = createFenceConnection(url);
        c->connect();
        printf("t1 start \n");
        c->acquireFence(createString("abc"));
        printf("t2 start \n");
        sleep(10);
        c->releaseFence(createString("abc"));
        printf("t3 start \n");
    });
    
    Thread t2 = createThread([&]{
        usleep(1000 * 1000);
        printf("start t2 \n");
        FenceConnection c = createFenceConnection(url);
        c->connect();
        
        TimeWatcher watch = createTimeWatcher();
        printf("start t2 acquire lock trace1 \n");
        watch->start();
        int ret = c->acquireFence(createString("abc"),3000);
        printf("start t2 acquire lock trace2 ,cost: %ld,ret is %d\n",watch->stop(),ret);
        c->releaseFence(createString("abc"));
    });
    
    t1->start();
    t2->start();
    
    t1->join();
    t2->join();
    
    setEnvPort(++port);
    sleep(1);
    return 0;
}