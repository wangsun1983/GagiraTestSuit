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
#include <signal.h>

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
    signal(SIGPIPE, SIG_IGN);
    int port = getEnvPort();
    String url = String::New("tcp://127.0.0.1:")->append(String::New(port));
    DistributeOption option = DistributeOption::New();
    option->setHandler(CenterHandler::New());
    
    QueueCenter center = QueueCenter::New(url,option);
    center->start();
    usleep(1000*100);
    QueueConnection c = QueueConnection::New(url);
    c->connect();

    Thread t = Thread::New([&]{
        usleep(1000*100);
        center->close();
    });
    
    Thread waitT1 = Thread::New([&] {
        auto watcher = TimeWatcher::New();
        watcher->start();
        auto result = c->get<TaskInfo>();
        auto cost = watcher->stop();
        
        if(cost > 120 || cost < 100) {
            TEST_FAIL("testQueue2ClientWaitCloseCenter case1,cost is %d",cost);
        }
        
        if(result->data != nullptr) {
            TEST_FAIL("testQueue2ClientWaitCloseCenter case2");
        }
        
        if(result->err != -ENETUNREACH) {
            TEST_FAIL("testQueue2ClientWaitCloseCenter case3,err is %d",result->err);
        }
    });
    
    Thread waitT2 = Thread::New([&] {
        auto watcher = TimeWatcher::New();
        watcher->start();
        auto result = c->get<TaskInfo>();
        auto cost = watcher->stop();
        
        if(cost > 120 || cost < 100) {
            TEST_FAIL("testQueue2ClientWaitCloseCenter case4");
        }
        
        if(result->data != nullptr) {
            TEST_FAIL("testQueue2ClientWaitCloseCenter case5");
        }
        
        if(result->err != -ENETUNREACH) {
            TEST_FAIL("testQueue2ClientWaitCloseCenter case6,err is %d",result->err);
        }
    });
    
    t->start();
    waitT1->start();
    waitT2->start();
    
    waitT1->join();
    waitT2->join();
    t->join();
    
    setEnvPort(++port);
    sleep(1);
    TEST_OK("testQueue2ClientWaitCloseCenter case100");
    return 0;
}
