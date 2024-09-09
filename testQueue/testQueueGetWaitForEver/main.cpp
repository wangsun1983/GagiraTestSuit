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
    
    QueueCenter center = QueueCenter::New(url,nullptr);
    center->start();
    usleep(1000*100);
      
    QueueConnection c = QueueConnection::New(url);
    c->connect();
    
    Thread t = Thread::New([&]{
        usleep(1000*100);
        QueueConnection c1 = QueueConnection::New(url);
        c1->connect();
        TaskInfo info = TaskInfo::New();
        info->name = String::New("abc");
        info->value = 100;
        usleep(1000*100);
        c1->add(info);
    });
    
    TimeWatcher watcher = TimeWatcher::New();
    watcher->start();
    t->start();
    auto result = c->get<TaskInfo>();
    auto cost = watcher->stop();
    
    if(cost > 240 || cost < 200 ) {
        TEST_FAIL("testQueueGetWaitForEver case1,cost is %d",cost);
    }
    
    if(result->data == nullptr) {
        TEST_FAIL("testQueueGetWaitForEver case2");
    }
    
    if(result->err != 0) {
        TEST_FAIL("testQueueGetWaitForEver case3");
    }
    
    auto info = result->data;
    if(!info->name->sameAs("abc")) {
        TEST_FAIL("testQueueGetWaitForEver case4");
    }
    
    if(info->value != 100) {
        TEST_FAIL("testQueueGetWaitForEver case5");
    }
    
    setEnvPort(++port);
    sleep(1);
    TEST_OK("testQueueGetWaitForEver case100");
    return 0;
}
