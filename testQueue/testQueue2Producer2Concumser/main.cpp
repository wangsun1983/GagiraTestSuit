#include <stdio.h>
#include <unistd.h>
#include <signal.h>

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

using namespace obotcha;
using namespace gagira;

DECLARE_CLASS(TaskInfo) IMPLEMENTS(Serializable){
public:
    String name;
    int value;
    DECLARE_REFLECT_FIELD(TaskInfo,name,value);
};

int main() {
    signal(SIGPIPE, SIG_IGN);
    int port = getEnvPort();
    String url = String::New("tcp://127.0.0.1:")->append(String::New(port));
    
    printf("trace1 \n");
    QueueCenter center = QueueCenter::New(url,nullptr);
    center->start();
    usleep(1000*100);
    
    int sum = 0;
    Thread t1 = Thread::New([&]{
        QueueConnection c = QueueConnection::New(url);
        c->connect();
        
        for(int i = 0;i < 256;i++) {
            TaskInfo task = TaskInfo::New();
            task->name = String::New("wangsl")->append(Integer::New(i)->toString());
            task->value = i;
            c->add(task);
        }
        sleep(20);
    });
    
    Thread t2 = Thread::New([&]{
        QueueConnection c = QueueConnection::New(url);
        c->connect();
        
        for(int i = 0;i < 256;i++) {
            TaskInfo task = TaskInfo::New();
            task->name = String::New("wangsl")->append(Integer::New(i)->toString());
            task->value = i;
            c->add(task);
        }
        sleep(20);
    });
    
    uint32_t t3_count = 0;
    Thread t3 = Thread::New([&]{
        QueueConnection c = QueueConnection::New(url);
        c->connect();
        
        while(1) {
            auto result = c->get<TaskInfo>();
            if(result->data == nullptr) {
                break;
            }
            
            auto task = result->data;
            auto id = task->value;
            auto name = String::New("wangsl")->append(Integer::New(id)->toString());
            if(!task->name->sameAs(name)) {
                TEST_FAIL("testQueue2Producer2Consumer case1");
                break;
            }
            
            t3_count++;
        }
    });
    
    uint32_t t4_count = 0;
    Thread t4 = Thread::New([&]{
        QueueConnection c = QueueConnection::New(url);
        c->connect();
        
        while(1) {
            auto result = c->get<TaskInfo>();
            if(result->data == nullptr) {
                break;
            }
            
            auto task = result->data;
            auto id = task->value;
            auto name = String::New("wangsl")->append(Integer::New(id)->toString());
            if(!task->name->sameAs(name)) {
                TEST_FAIL("testQueue2Producer2Consumer case1");
                break;
            }
            
            t4_count++;
        }
    });
    
    t1->start();
    t2->start();
    t3->start();
    t4->start();
    
    t1->join();
    t2->join();
    center->close();
    
    t3->join();
    t4->join();
    
    if((t3_count + t4_count) != 512) {
        TEST_FAIL("testQueue2Producer2Consumer case1,count is %ld",t3_count + t4_count);
    }
    
    if(center->getRemainTaskSize() != 0) {
        TEST_FAIL("testQueue2Producer2Consumer case2,remain task size is %d",center->getRemainTaskSize());
    }
    
    if(center->getRemainWaiterSize() != 0) {
        TEST_FAIL("testQueue2Producer2Consumer case3,remain waiter size is %d",center->getRemainWaiterSize());
    }

    TEST_OK("testQueue2Producer2Consumer case100");
    
    setEnvPort(++port);
    sleep(1);
    return 0;
}
