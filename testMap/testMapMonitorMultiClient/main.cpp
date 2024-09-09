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

using namespace obotcha;
using namespace gagira;

DECLARE_CLASS(StudentInfo) IMPLEMENTS(Serializable){
public:
    String name;
    int age;
    DECLARE_REFLECT_FIELD(StudentInfo,name,age);
};

AtomicInteger count = AtomicInteger::New(0);

DECLARE_CLASS(MyListener) IMPLEMENTS(MapConnectionListener) {
public:
    void onConnect() {
        //TODO
    }
    
    void onDisconnect() {
        //TODO
    }
    
    void onEvent(int event,String tag,ByteArray data) {
        count->incrementAndGet();
        if(!tag->sameAs("Student")) {
            TEST_FAIL("testMapMonitorTwice case1,tag is %s",tag->toChars());
        }
        
        StudentInfo info = StudentInfo::New();
        info->deserialize(data);
        
        if(!info->name->sameAs("wang")) {
            TEST_FAIL("testMapMonitorTwice case2");
        }
        
        if(info->age != 1122) {
            TEST_FAIL("testMapMonitorTwice case3");
        }
    }
};

int main() {
    int port = getEnvPort();
    String url = String::New("tcp://127.0.0.1:")->append(String::New(port));
    
    MapCenter center = MapCenter::New(url,nullptr);
    center->start();
    usleep(1000*100);
    ArrayList<MapConnection> conns = ArrayList<MapConnection>::New();
    
    for(int i = 0;i < 64;i++) {
    auto listener = MyListener::New();
        MapConnection conn = MapConnection::New(url,listener);
        conn->connect();
        conn->monitor(String::New("Student"));
        conns->add(conn);
    }
    usleep(1000*100);
    
    StudentInfo info = StudentInfo::New();
    info->name = String::New("wang");
    info->age = 1122;
    
    MapConnection conn2 = MapConnection::New(url,nullptr);
    conn2->connect();    
    conn2->update(String::New("Student"),info);
    usleep(1000*500);
    
    auto c = count->get();
    if(c != 64) {
        TEST_FAIL("testMapMonitorMultiClient case1,c is %d",c);
    }
    
    setEnvPort(++port);
    sleep(1);
    TEST_OK("testMapMonitorMultiClient case100");
    return 0;
}
