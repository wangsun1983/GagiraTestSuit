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
    
    uint32_t vv = 0;
    int sum = 0;
    Thread t1 = Thread::New([&]{
        QueueConnection c = QueueConnection::New(url);
        c->connect();
        
        for(int i = 0;i < 256;i++) {
            TaskInfo task = TaskInfo::New();
            task->name = String::New("wangsl")->append(Integer::New(i)->toString());
            task->value = i;
            c->add(task);
            vv += i;
        }
        sleep(10);
        printf("t1 start close \n");
        center->close();
    });
    
    uint32_t t2_count = 0;
    Thread t2 = Thread::New([&]{
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
                TEST_FAIL("testQueueMultiClient case1");
                break;
            }
            
            t2_count += id;
        }
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
                TEST_FAIL("testQueueMultiClient case1");
                break;
            }
            
            t3_count += id;
        }
    });
    
    t1->start();
    t2->start();
    t3->start();
    
    t1->join();
    t2->join();
    t3->join();
    
    if(vv != (t3_count + t2_count)) {
        TEST_FAIL("testQueueMultiClient case1,vv is %ld,t3_count is %ld,t2_count is %ld",vv,t3_count,t2_count);
    }
    TEST_OK("testQueueMultiClient case100");
    
    setEnvPort(++port);
    sleep(1);
    return 0;
}
