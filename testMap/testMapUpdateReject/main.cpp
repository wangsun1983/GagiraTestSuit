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


DECLARE_CLASS(MyHandler) IMPLEMENTS(DistributeHandler) {
public:
    int onRequest(DistributeLinker linker,MapMessage msg) {
        switch(msg->getEvent()) {
            case st(MapMessage)::Type::Update: {
                if(count == 0) {
                    count++;
                    return 0;
                }
                
                if(msg->getTag()->sameAs("abc")) {
                    return -1;
                }
                break;
            }
        }
        
        return 0;
    }

private:
    int count = 0;
};

int main() {
    int port = getEnvPort();
    String url = String::New("tcp://127.0.0.1:")->append(String::New(port));
    auto option = MapOption::New();
    option->setHandler(MyHandler::New());
    MapCenter center = MapCenter::New(url,option);
    center->start();
    usleep(1000*100);

    MapConnection connection = MapConnection::New(url);
    int ret = connection->connect();
    StudentInfo info = StudentInfo::New();
    info->name = String::New("wang");
    info->age = 1;
    ret = connection->update(String::New("abc"),info);
    if(ret != 0) {
        TEST_FAIL("testMapUpdateReject,case1");
    }
    
    usleep(1000*50);
    StudentInfo info2 = StudentInfo::New();
    info2->name = String::New("abc");
    info2->age = 100;
    ret = connection->update(String::New("abc"),info);
    if(ret != -EPERM) {
        TEST_FAIL("testMapUpdateReject,case2,ret is %d",ret);
    }
    
    ret = connection->update(String::New("abc"),info);
    if(ret != -EPERM) {
        TEST_FAIL("testMapUpdateReject,case3,ret is %d",ret);
    }
    
    auto result = connection->get<StudentInfo>(String::New("abc"));
    if(!result->name->sameAs("wang")) {
        TEST_FAIL("testMapUpdateReject,case4,name is %s",result->name->toChars());
    }
    
    if(result->age != 1) {
        TEST_FAIL("testMapUpdateReject,case5,age is %d",result->age);
    }
    
    setEnvPort(++port);
    TEST_OK("testMapUpdateReject,case100");
    return 0;
}
