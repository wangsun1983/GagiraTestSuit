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
    void onEvent(int event,String tag,ByteArray data) {
        if(count == 0 && event != Event::Create) {
            TEST_FAIL("testMapOnDataUpdateFirstCreate,case1");
        }
        
        if(!tag->sameAs("Student")) {
            TEST_FAIL("testMapOnDataUpdateFirstCreate,case3");
        }
        
        StudentInfo info = StudentInfo::New();
        info->deserialize(data);
        
        if(!info->name->sameAs("wangssss")) {
            TEST_FAIL("testMapOnDataUpdateFirstCreate,case4");
        }
        
        if(info->age != 21) {
            TEST_FAIL("testMapOnDataUpdateFirstCreate,case5,info age is %d,count is %d",
                info->age,21);
        }
        
        latch->countDown();
    }
    
    int count = 0;
};

int main() {
    int port = getEnvPort();
    String url = String::New("tcp://127.0.0.1:")->append(String::New(port));
    MapCenter center = MapCenter::New(url,nullptr);
    center->start();
    usleep(1000*100);
    auto listener = MyListener::New();
    
    Thread t1 = Thread::New([&]{
        MapConnection conn = MapConnection::New(url,listener);
        conn->connect();
        
        conn->monitor(String::New("Student"));
        latch->await();
    });
    t1->start();
    usleep(1000*100);
    
    StudentInfo info = StudentInfo::New();
    info->name = String::New("wangssss");
    info->age = 21;
    
    MapConnection conn2 = MapConnection::New(url,nullptr);
    conn2->connect();    
    conn2->update(String::New("Student"),info);
    
    t1->join();
    setEnvPort(++port);
    sleep(1);
    TEST_OK("testMapOnDataUpdateFirstCreate,case100");
    return 0;
}
