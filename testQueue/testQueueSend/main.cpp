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
    QueueCenter center = createQueueCenter(url,nullptr);
    center->start();
    usleep(1000*100);
    
    uint32_t vv = 0;
    int sum = 0;
    Thread t1 = Thread::New([&]{
        QueueConnection c = createQueueConnection(url);
        c->connect();
        
        for(int i = 0;i < 8;i++) {
            TaskInfo task = createTaskInfo();
            task->name = String::New("wangsl");
            task->value = i;
            c->submitTask(task);
            vv += i;
        }
        sleep(1);
    });
    
    Thread t2 = Thread::New([&]{
        QueueConnection c = createQueueConnection(url);
        c->connect();
        
        for(int i = 0;i < 8;i++) {
            printf("thread t2 trace1 \n");
            auto task = c->acquireTask<TaskInfo>();
            printf("thread t2 trace2 \n");
            sum += task->value;
        }
        sleep(1);
    });
    
    t1->start();
    t2->start();
    
    t1->join();
    t2->join();
    
    printf("sum is %d,vv iss %d \n",sum,vv);
    
    setEnvPort(++port);
    sleep(1);
    return 0;
}
