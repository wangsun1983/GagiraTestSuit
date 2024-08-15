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
    
    auto pid = fork();
    if(pid == 0) {
        usleep(1000*100);
        FenceConnection c = FenceConnection::New(url);
        c->connect();
        auto ret = c->acquireFence(String::New("abc"));
        usleep(1000*100);
        c->close();
        exit(0);
    } else {
        auto option = FenceOption::New();
        FenceCenter center = FenceCenter::New(url,option);
        center->start();
        usleep(1000*300);
        FenceConnection c = FenceConnection::New(url);
        c->connect();
        auto ret = c->acquireFence(String::New("abc"));
        if(ret != 0) {
            TEST_FAIL("testFenceAcquire Release after process exit case1,ret is %d",ret);
        }
        center->close();
    }
    setEnvPort(++port);
    TEST_OK("testFenceAcquire Release after process exit case100");
    return 0;
}
