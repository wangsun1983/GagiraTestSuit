#include <stdio.h>
#include <unistd.h>

#include "Controller.hpp"
#include "Server.hpp"
#include "Inet4Address.hpp"
#include "ServletRequest.hpp"
#include "HttpResourceManager.hpp"
#include "Reflect.hpp"
#include "Utils.hpp"
#include "MqCenter.hpp"
#include "MqConnection.hpp"
#include "Serializable.hpp"
#include "CountDownLatch.hpp"
#include "TestLog.hpp"
#include "Handler.hpp"
#include "NetPort.hpp"
#include "Thread.hpp"
#include "SpaceConnection.hpp"
#include "SpaceCenter.hpp"
#include "CountDownLatch.hpp"

using namespace obotcha;
using namespace gagira;

CountDownLatch latch = createCountDownLatch(1);

DECLARE_CLASS(StudentInfo) IMPLEMENTS(Serializable){
public:
    String name;
    int age;
    DECLARE_REFLECT_FIELD(StudentInfo,name,age);
};

DECLARE_CLASS(MyListener) IMPLEMENTS(SpaceConnectionListener) {
public:
    void onConnect() {
        //TODO
    }
    
    void onDisconnect() {
        //TODO
    }
    
    void onDataUpdate(String tag,ByteArray data) {
        if(!tag->equals("Student")) {
            printf("tag is error!!! \n");
        }
        
        StudentInfo info = createStudentInfo();
        info->deserialize(data);
        
        if(!info->name->equals("wang")) {
            printf("name is error!!! \n");
        }
        
        if(info->age != 1122) {
            printf("age is error!!! \n");
        }
        
        printf("all is ok \n");
        latch->countDown();
    }
    
};

int main() {
    int port = getEnvPort();
    String url = String::New("tcp://127.0.0.1:")->append(String::New(port));
    
    printf("trace1 \n");
    SpaceCenter center = createSpaceCenter(url,nullptr);
    center->start();
    usleep(1000*100);
    
    Thread t1 = Thread::New([&]{
        SpaceConnection conn = createSpaceConnection(url,createMyListener());
        conn->connect();
        
        conn->monitor(String::New("Student"));
        latch->await();
    });
    t1->start();
    usleep(1000*100);
    
    StudentInfo info = createStudentInfo();
    info->name = String::New("wang");
    info->age = 1122;
    
    SpaceConnection conn2 = createSpaceConnection(url,nullptr);
    conn2->connect();    
    conn2->update(String::New("Student"),info);
    t1->join();
    
    setEnvPort(++port);
    sleep(1);
    return 0;
}
