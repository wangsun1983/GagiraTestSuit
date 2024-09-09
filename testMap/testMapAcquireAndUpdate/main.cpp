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
    auto option = MapOption::New();
    MapCenter center = MapCenter::New(url,nullptr);
    center->start();
    usleep(1000*100);

    MapConnection connection = MapConnection::New(url);
    int ret = connection->connect();
    StudentInfo info = StudentInfo::New();
    info->name = String::New("wang");
    info->age = 1;
    ret = connection->update(String::New("abc"),info);
    if(ret != 0) {
        TEST_FAIL("testMapAcquireAndUpdate,case1,ret is %d",ret);
    }
    
    usleep(1000*50);
    auto retvalue = connection->get<StudentInfo>(String::New("abc"));
    if(retvalue == nullptr) {
        TEST_FAIL("testMapAcquireAndUpdate,case2");
    }
    
    if(connection->getErr() != 0) {
        TEST_FAIL("testMapAcquireAndUpdate,case3,ret is %d",ret);
    }
    
    if(retvalue->age != 1) {
        TEST_FAIL("testMapAcquireAndUpdate,case4,age is %d",retvalue->age);
    }
    
    if(retvalue->name == nullptr || !retvalue->name->sameAs("wang")) {
        TEST_FAIL("testMapAcquireAndUpdate,case5,name is %s",retvalue->name->toChars());
    }
    
    //update again
    info = StudentInfo::New();
    info->name = String::New("sun");
    info->age = 2;
    ret = connection->update(String::New("abc"),info);
    
    retvalue = connection->get<StudentInfo>(String::New("abc"));
    if(retvalue == nullptr) {
        TEST_FAIL("testMapAcquireAndUpdate,case6");
    }
    
    if(connection->getErr() != 0) {
        TEST_FAIL("testMapAcquireAndUpdate,case7,ret is %d",ret);
    }
    
    if(retvalue->age != 2) {
        TEST_FAIL("testMapAcquireAndUpdate,case8,age is %d",retvalue->age);
    }
    
    if(retvalue->name == nullptr || !retvalue->name->sameAs("sun")) {
        TEST_FAIL("testMapAcquireAndUpdate,case9,name is %s",retvalue->name->toChars());
    }
    
    setEnvPort(++port);
    TEST_OK("testMapAcquireAndUpdate,case100");
    return 0;
}
