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
#include "TimeWatcher.hpp"

using namespace obotcha;
using namespace gagira;

DECLARE_CLASS(TaskInfo) IMPLEMENTS(Serializable){
public:
    String name;
    int value;
    DECLARE_REFLECT_FIELD(TaskInfo,name,value);
};

int main() {
    int port = getEnvPort();
    String url = String::New("tcp://127.0.0.1:")->append(String::New(port));
    
    printf("trace1 \n");
    QueueCenter center = QueueCenter::New(url,nullptr);
    center->start();
    usleep(1000*100);
      
    QueueConnection c = QueueConnection::New(url);
    c->connect();
    TimeWatcher watcher = TimeWatcher::New();
    watcher->start();
    auto result = c->get<TaskInfo>(100);
    auto cost = watcher->stop();
    
    if(cost > 130 || cost < 95 ) {
        TEST_FAIL("testQueueWaitEmptyTimeOut case1,cost is %d",cost);
    }
    
    if(result->data != nullptr) {
        TEST_FAIL("testQueueWaitEmptyTimeOut case2");
    }
    
    if(result->err != -ETIMEDOUT) {
        TEST_FAIL("testQueueWaitEmptyTimeOut case3");
    }
    
    setEnvPort(++port);
    sleep(1);
    TEST_OK("testQueueWaitEmptyTimeOut case100");
    return 0;
}
