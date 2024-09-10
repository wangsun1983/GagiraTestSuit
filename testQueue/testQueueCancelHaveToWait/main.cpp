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

bool isHit = false;
int count = 0;

DECLARE_CLASS(CenterHandler) IMPLEMENTS(DistributeHandler) {
public:
    int onRequest(DistributeLinker linker,QueueMessage msg) {
        return 0;
    }
};

int main() {
    int port = getEnvPort();
    String url = String::New("tcp://127.0.0.1:")->append(String::New(port));
    DistributeOption option = DistributeOption::New();
    option->setHandler(CenterHandler::New());
    
    QueueCenter center = QueueCenter::New(url,option);
    center->setDelayProcessInterval(1000);
    
    center->start();
    usleep(1000*100);
    
    Thread t1 = Thread::New([&]{
        QueueConnection c = QueueConnection::New(url);
        c->connect();
        auto watcher = TimeWatcher::New();
        watcher->start();
        auto result = c->get<TaskInfo>(500);
        auto cost = watcher->stop();
        if(result->data == nullptr) {
            TEST_FAIL("testQueueCancelHaveToWait case1");
        }
        
        auto info = result->data;
        if(!info->name->sameAs("abc")) {
            TEST_FAIL("testQueueCancelHaveToWait case2");
        }
        
        if(info->value != 123) {
            TEST_FAIL("testQueueCancelHaveToWait case3");
        }
        
        if(cost < 700) {
            TEST_FAIL("testQueueCancelHaveToWait case4");
        }
    });
    
    Thread t2 = Thread::New([&]{
        usleep(1000*100);
        TaskInfo info = TaskInfo::New();
        info->name = String::New("abc");
        info->value = 123;
        
        QueueConnection c = QueueConnection::New(url);
        c->connect();
        c->add(info);
        usleep(1000*50);
    });
    t1->start();
    t2->start();
    
    t1->join();
    t2->join();
    
  
    setEnvPort(++port);
    sleep(1);
    TEST_OK("testQueueCancelHaveToWait case100");
    return 0;
}
