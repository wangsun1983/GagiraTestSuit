#include <stdio.h>
#include <unistd.h>

#include "Controller.hpp"
#include "Server.hpp"
#include "Inet4Address.hpp"
#include "ServletRequest.hpp"
#include "HttpResourceManager.hpp"
#include "Reflect.hpp"
#include "Utils.hpp"
#include "MapCenter.hpp"
#include "Serializable.hpp"
#include "CountDownLatch.hpp"
#include "TestLog.hpp"
#include "Handler.hpp"
#include "NetPort.hpp"
#include "Thread.hpp"
#include "MapConnection.hpp"
#include "CountDownLatch.hpp"
#include "TimeWatcher.hpp"

using namespace obotcha;
using namespace gagira;

CountDownLatch latch = CountDownLatch::New(1);

DECLARE_CLASS(StudentInfo) IMPLEMENTS(Serializable){
public:
    String name;
    int age;
    DECLARE_REFLECT_FIELD(StudentInfo,name,age);
};

DECLARE_CLASS(MyListener) IMPLEMENTS(MapConnectionListener) {
public:
    void onConnect() {
    }
    
    void onDisconnect() {
        latch->countDown();
    }
    
    void onEvent(int event,String tag,ByteArray data) {
        
    }
    
};

int main() {
    int port = getEnvPort();
    String url = String::New("tcp://127.0.0.1:")->append(String::New(port));

    MapCenter center = MapCenter::New(url,nullptr);
    center->start();
    usleep(1000*100);
    
    Thread t1 = Thread::New([&]{
        MapConnection conn = MapConnection::New(url,MyListener::New());
        conn->connect();
        
        conn->monitor(String::New("Student"));
        latch->await();
    });
    t1->start();
    usleep(1000*100);
    
    StudentInfo info = StudentInfo::New();
    info->name = String::New("wang");
    info->age = 1122;
    
    usleep(100*1000);
    center->close();
    
    auto watch = TimeWatcher::New();
    watch->start();
    t1->join(5000);
    auto cost = watch->stop();
    if(cost > 25) {
        TEST_FAIL("testmapDisConnectCallback case2,cost is %d",cost);
    }
    
    setEnvPort(++port);
    sleep(1);
    TEST_OK("testmapDisConnectCallback case100");
    return 0;
}
