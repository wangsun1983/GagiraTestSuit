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


DECLARE_CLASS(MyHandler) IMPLEMENTS(DistributeHandler) {
    int onRequest(DistributeLinker,sp<_FenceMessage> msg) {
        printf("name is %s trace1 \n",msg->getFenceName()->toChars());
        if(msg->getFenceName()->sameAs("abc")) {
            printf("name is %s trace2 \n",msg->getFenceName()->toChars());
            return  -123;
        } else if(msg->getFenceName()->sameAs("efg")){
            printf("name is %s trace3 \n",msg->getFenceName()->toChars());
            return  -456;
        }
        printf("name is %s trace4 \n",msg->getFenceName()->toChars());
        return 0;
    }
};

int main() {
    int port = getEnvPort();
    String url = String::New("tcp://127.0.0.1:")->append(String::New(port));
    auto option = FenceOption::New();
    option->setHandler(MyHandler::New());
    FenceCenter center = FenceCenter::New(url,option);
    center->start();
    usleep(1000*100);
    
    FenceConnection c = FenceConnection::New(url);
    c->connect();
    auto ret = c->acquireFence(String::New("abc"));
    if(ret != -123) {
        TEST_FAIL("testFenceAcquire Reject case1,ret is %d",ret);
    }
    
    ret = c->acquireFence(String::New("efg"));
    if(ret != -456) {
        TEST_FAIL("testFenceAcquire Reject case2,ret is %d",ret);
    }
    
    printf("test trace1_0 \n");
    ret = c->acquireFence(String::New("aaa"));
    printf("test trace1_1 \n");
    if(ret != 0) {
        TEST_FAIL("testFenceAcquire Reject case3,ret is %d",ret);
    }
    
    FenceConnection c2 = FenceConnection::New(url);
    c2->connect();
    printf("test trace1_2 \n");
    Thread t = Thread::New([&]{
        printf("test thread trace1 \n");
        usleep(1000 * 200);
        c->releaseFence(String::New("aaa"));
        printf("test thread trace2 \n");
    });
    t->start();
    
    printf("test trace1 \n");
    TimeWatcher watch = TimeWatcher::New();
    watch->start();
    printf("test trace2 \n");
    ret = c2->acquireFence(String::New("aaa"));
    printf("test trace3 \n");
    auto cost = watch->stop();
    if(ret != 0) {
        TEST_FAIL("testFenceAcquire Reject case4,ret is %d",ret);
    }
    
    if(cost > 220) {
        TEST_FAIL("testFenceAcquire Reject case5,cost is %d",cost);
    }

    t->join();
    
    setEnvPort(++port);
    TEST_OK("testFenceAcquire Reject case100");
    return 0;
}
