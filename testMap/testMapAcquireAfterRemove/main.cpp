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
        TEST_FAIL("testMapAcquireAfterRemove,case1,ret is %d",ret);
    }
    
    usleep(1000*50);
    connection->remove(String::New("abc"));
    
    connection->remove(String::New("abc"));
    auto retvalue = connection->get<StudentInfo>(String::New("abc"));
    if(retvalue != nullptr) {
        TEST_FAIL("testMapAcquireAfterRemove,case2");
    }
    
    auto err = connection->getErr();
    if(err != -ENODATA) {
        TEST_FAIL("testMapAcquireAfterRemove,case3");
    }
    setEnvPort(++port);
    TEST_OK("testMapAcquireAfterRemove,case100");
    return 0;
}
