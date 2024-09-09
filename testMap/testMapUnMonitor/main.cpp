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
#include "MapCenter.hpp"
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
        //TODO
    }
    
    void onDisconnect() {
        //TODO
    }
    
    void onEvent(int event,String tag,ByteArray data) {
        if(count == 0 && event == Event::Create) {
            if(!tag->sameAs("Student")) {
                TEST_FAIL("testMapUnMonitor case1");
            }
            
            if(data == nullptr) {
                TEST_FAIL("testMapUnMonitor case2");
                return;
            }
            
            StudentInfo info = StudentInfo::New();
            info->deserialize(data);
            
            if(!info->name->sameAs("wang")) {
                TEST_FAIL("testMapUnMonitor case3");
                return;
            }
            
            if(info->age != 1122) {
                TEST_FAIL("testMapUnMonitor case4");
                return;
            }
            latch->countDown();
            count++;
        } else if(count == 1) {
            TEST_FAIL("testMapUnMonitor case5");
        }
    }

    int count = 0;
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
        
        conn->unMonitor(String::New("Student"));
        latch = CountDownLatch::New(1);
        usleep(1000*2000);
    });
    t1->start();
    usleep(1000*100);
    
    StudentInfo info = StudentInfo::New();
    info->name = String::New("wang");
    info->age = 1122;
    
    MapConnection conn2 = MapConnection::New(url,nullptr);
    conn2->connect();     
    conn2->update(String::New("Student"),info);
    
    usleep(1000*300);
    conn2->update(String::New("Student"),info);
    TimeWatcher watcher = TimeWatcher::New();
    watcher->start();
    latch->await(1000);
    auto internal = watcher->stop();
    if(internal < 995 || internal > 1005) {
        TEST_FAIL("testMapUnMonitor case6,interval is %d \n",internal);
    }
    
    setEnvPort(++port);
    sleep(1);
    TEST_OK("testMapUnMonitor case100");
    return 0;
}
