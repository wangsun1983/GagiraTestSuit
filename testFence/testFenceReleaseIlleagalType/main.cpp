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

int main() {
    int port = getEnvPort();
    String url = String::New("tcp://127.0.0.1:")->append(String::New(port));

    FenceCenter center = FenceCenter::New(url,nullptr);
    center->start();
    usleep(1000*100);
    FenceConnection c1 = FenceConnection::New(url);
    c1->connect();
    int ret = c1->acquireFence(String::New("abc"));
    if(ret != 0) {
        TEST_FAIL("testFence Release illeagal type case1,ret is %d",ret);
    }
    
    ret = c1->releaseReadFence(String::New("abc"));
    if(ret != -EINVAL) {
        TEST_FAIL("testFence Release illeagal type case2,ret is %d",ret);
    }
    
    ret = c1->releaseWriteFence(String::New("abc"));
    if(ret != -EINVAL) {
        TEST_FAIL("testFence Release illeagal type case3,ret is %d",ret);
    }
    
    ret = c1->releaseFence(String::New("abc"));
    if(ret != 0) {
        TEST_FAIL("testFence Release illeagal type case4,ret is %d",ret);
    }
    
    int ret2 = c1->acquireReadFence(String::New("abc1"));
    if(ret2 != 0) {
        TEST_FAIL("testFence Release illeagal type case5,ret2 is %d",ret2);
    }
    
    ret2 = c1->releaseFence(String::New("abc1"));
    if(ret2 != -EINVAL) {
        TEST_FAIL("testFence Release illeagal type case6,ret2 is %d",ret2);
    }
    
    ret2 = c1->releaseWriteFence(String::New("abc1"));
    if(ret2 != -ECANCELED) {
        TEST_FAIL("testFence Release illeagal type case6_1,ret2 is %d",ret2);
    }
    
    ret2 = c1->releaseReadFence(String::New("abc1"));
    if(ret2 != 0) {
        TEST_FAIL("testFence Release illeagal type case7,ret2 is %d",ret2);
    }
    
    int ret3 = c1->acquireWriteFence(String::New("abc2"));
    ret3 = c1->releaseFence(String::New("abc2"));
    if(ret3 != -EINVAL) {
        TEST_FAIL("testFence Release illeagal type case8,ret3 is %d",ret3);
    }
    
    ret3 = c1->releaseReadFence(String::New("abc2"));
    if(ret3 != -ECANCELED) {
        TEST_FAIL("testFence Release illeagal type case9,ret3 is %d",ret3);
    }
    
    ret3 = c1->releaseWriteFence(String::New("abc2"));
    if(ret3 != 0) {
        TEST_FAIL("testFence Release illeagal type case10,ret3 is %d",ret3);
    }
    
    setEnvPort(++port);
    TEST_OK("testFence Release illeagal type case100");
    return 0;
}
