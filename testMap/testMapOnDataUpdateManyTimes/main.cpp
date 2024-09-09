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

CountDownLatch latch = CountDownLatch::New(1024*4);

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
            TEST_FAIL("testMapOnDataUpdateManyTimes,case1");
        }
        
        if(count != 0  && event != Event::Update) {
            TEST_FAIL("testMapOnDataUpdateManyTimes,case2");
        }
        
        if(!tag->sameAs("Student")) {
            TEST_FAIL("testMapOnDataUpdateManyTimes,case3");
        }
        
        StudentInfo info = StudentInfo::New();
        info->deserialize(data);
        
        auto countstr = Integer::New(count)->toString();
        
        if(!info->name->sameAs(countstr)) {
            TEST_FAIL("testMapOnDataUpdateManyTimes,case4,info name is %s,expect is %s",
                info->name->toChars(),countstr->toChars());
        }
        
        if(info->age != count) {
            TEST_FAIL("testMapOnDataUpdateManyTimes,case5,info age is %d,count is %d",
                info->age,count);
        }
        
        count++;
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
    info->name = String::New("wang");
    info->age = 1;
    
    MapConnection conn2 = MapConnection::New(url,nullptr);
    conn2->connect();    
    for(int i = 0;i < 1024*4;i++) {
        info->age = i;
        info->name = Integer::New(i)->toString();
        conn2->update(String::New("Student"),info);
    }
    
    Thread t2 = Thread::New([&]{
        sleep(5);
        printf("listener count is %d,latch is %d \n",listener->count,latch->getCount());
    });
    t2->start();
    
    t1->join();
    setEnvPort(++port);
    sleep(1);
    TEST_OK("testMapOnDataUpdateManyTimes,case100");
    return 0;
}
