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
    TaskInfo info = TaskInfo::New();
    info->name = String::New("abc");
    info->value = 100;
    c->add(info);
    usleep(1000*100);
    TimeWatcher watcher = TimeWatcher::New();
    watcher->start();
    auto result = c->get<TaskInfo>(100);
    auto cost = watcher->stop();
    printf("trace3 \n");
    if(cost > 20) {
        TEST_FAIL("testQueueGetDataWithTimeOut case1,cost is %d",cost);
    }
    
    if(result->data == nullptr) {
        TEST_FAIL("testQueueGetDataWithTimeOut case2");
    }

    auto task = result->data;
    if(task->value != 100) {
        TEST_FAIL("testQueueGetDataWithTimeOut case3");
    }
    
    if(task->name == nullptr || !task->name->sameAs("abc")) {
        TEST_FAIL("testQueueGetDataWithTimeOut case4");
    }
    
    if(result->err != 0) {
        TEST_FAIL("testQueueGetDataWithTimeOut case5");
    }
    
    setEnvPort(++port);
    sleep(1);
    TEST_OK("testQueueGetDataWithTimeOut case100");
    return 0;
}
