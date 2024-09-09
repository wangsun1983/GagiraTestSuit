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
        if(event == st(QueueMessage)::EventType::Submit && count == 0) {
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
    
    TaskInfo info = TaskInfo::New();
    info->name = String::New("abc");
    info->value = 100;
    
    auto result = c->add(info);
    if(result != -EPERM) {
        TEST_FAIL("testQueueSubmitReject case1,result is %d",result);
    } 
    
    auto val = c->get<TaskInfo>(100);
    if(val->data != nullptr) {
        TEST_FAIL("testQueueSubmitReject case2,result is %d",result);
    }
    
    if(val->err != -ETIMEDOUT) {
        TEST_FAIL("testQueueSubmitReject case3,err is %d",val->err);
    }
    
    result = c->add(info);
    if(result != 0) {
        TEST_FAIL("testQueueSubmitReject case4,result is %d",result);
    }
    
    auto watcher = TimeWatcher::New();
    watcher->start();
    val = c->get<TaskInfo>(100);
    auto cost = watcher->stop();
    if(cost > 30) {
        TEST_FAIL("testQueueSubmitReject case5,cost is %d",cost);
    }
    
    if(val->err != 0) {
        TEST_FAIL("testQueueSubmitReject case6,err is %d",val->err);
    }
    
    auto task = val->data;
    if(task == nullptr) {
        TEST_FAIL("testQueueSubmitReject case7");
    }
    
    if(!task->name->sameAs("abc")) {
        TEST_FAIL("testQueueSubmitReject case8");
    }
    
    if(task->value != 100) {
        TEST_FAIL("testQueueSubmitReject case9");
    }
    
    setEnvPort(++port);
    sleep(1);
    TEST_OK("testQueueSubmitReject case100");
    return 0;
}
