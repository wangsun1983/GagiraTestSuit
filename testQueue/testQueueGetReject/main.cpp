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
        auto event = msg->getEvent();
        if(event == st(QueueMessage)::EventType::Acquire && count == 0) {
            isHit = true;
            count++;
            return -1;
        }
        
        return 0;
    }
};

int main() {
    int port = getEnvPort();
    String url = String::New("tcp://127.0.0.1:")->append(String::New(port));
    DistributeOption option = DistributeOption::New();
    option->setHandler(CenterHandler::New());
    
    QueueCenter center = QueueCenter::New(url,option);
    center->start();
    usleep(1000*100);
    QueueConnection c = QueueConnection::New(url);
    c->connect();
    
    auto result = c->get<TaskInfo>(100);
     
    if(result->data != nullptr) {
        TEST_FAIL("testQueueGetReject case1");
    }
    
    if(result->err != -EPERM) {
        TEST_FAIL("testQueueGetReject case2");
    }
    
    if(!isHit) {
        TEST_FAIL("testQueueGetReject case3");
    }
    
    //try get again
    auto watcher = TimeWatcher::New();
    watcher->start();
    result = c->get<TaskInfo>(100);
    auto cost = watcher->stop();
    if(cost > 130 || cost < 100) {
        TEST_FAIL("testQueueGetReject case4,cost is %d",cost);
    }
    
    if(result->data != nullptr) {
        TEST_FAIL("testQueueGetReject case5");
    }
    
    if(result->err != -ETIMEDOUT) {
        TEST_FAIL("testQueueGetReject case6,result->err is %d",result->err);
    }
    
    setEnvPort(++port);
    sleep(1);
    TEST_OK("testQueueGetReject case100");
    return 0;
}
