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
        TEST_FAIL("testFence Acquire illeagal type ReadWrite case1,ret is %d",ret);
    }
    
    int ret2 = c1->acquireReadFence(String::New("abc"));
    if(ret2 != -EINVAL) {
        TEST_FAIL("testFence Acquire illeagal type ReadWrite case2,ret2 is %d",ret2);
    }
    c1->releaseFence(String::New("abc"));
    
    ret2 = c1->acquireReadFence(String::New("abc1"));
    
    int ret3 = c1->acquireFence(String::New("abc1"));
    if(ret3 != -EINVAL) {
        TEST_FAIL("testFence Acquire illeagal type ReadWrite case3,ret3 is %d",ret3);
    }

    setEnvPort(++port);
    TEST_OK("testFence Acquire illeagal type ReadWrite case100");
    return 0;
}
