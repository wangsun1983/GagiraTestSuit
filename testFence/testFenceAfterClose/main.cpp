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
    
    c1->close();
    auto ret = c1->acquireFence(String::New("abc"));
    if(ret != -ENETUNREACH) {
        TEST_FAIL("testFenceAcquire Acquire After close case1,ret is %d",ret);
    }
   
    setEnvPort(++port);
    TEST_OK("testFenceAcquire Acquire After close case100");
    return 0;
}
