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

using namespace obotcha;
using namespace gagira;

CountDownLatch latch = CountDownLatch::New(1024*32);

DECLARE_CLASS(StudentInfo) IMPLEMENTS(Serializable){
public:
    String name;
    int age;
    DECLARE_REFLECT_FIELD(StudentInfo,name,age);
};

int main() {
    int port = getEnvPort();
    String url = String::New("tcp://127.0.0.1:")->append(String::New(port));
    MapCenter center = MapCenter::New(url,nullptr);
    center->start();
    usleep(1000*100);
    MapConnection connection = MapConnection::New(url);
    int ret = connection->connect();
    
    for(int i = 0;i < 1024;i++) {
        StudentInfo info = StudentInfo::New();
        info->name = String::New("wang")->append(Integer::New(i)->toString());
        info->age = i;
        int ret = connection->update(String::New("Student"),info);
        if(ret != 0) {
            TEST_FAIL("testMapUpdate100Times,case1");
            return 0;
        }
    }
    
    usleep(1000*50);
    auto result = connection->get<StudentInfo>(String::New("Student"));
    if(!result->name->sameAs("wang1023")) {
        TEST_FAIL("testMapUpdate100Times,case2,name is %s",result->name->toChars());
    }
    
    if(result->age != 1023) {
        TEST_FAIL("testMapUpdate100Times,case3");
    }
    
    setEnvPort(++port);
    TEST_OK("testMapUpdate100Times,case100");
    return 0;
}
