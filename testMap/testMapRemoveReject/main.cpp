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
            case st(MapMessage)::Type::Remove: {
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
        TEST_FAIL("testMapRemoveReject,case1,ret is %d",ret);
    }
    
    usleep(1000*50);
    auto retvalue = connection->remove(String::New("abc"));
    if(retvalue != -EPERM) {
        TEST_FAIL("testMapRemoveReject,case3,ret is %d",ret);
    }
    
    auto chkvalue = connection->get<StudentInfo>(String::New("abc"));
    if(chkvalue == nullptr) {
        TEST_FAIL("testMapRemoveReject,case4");
    }
    
    if(!chkvalue->name->sameAs("wang")) {
        TEST_FAIL("testMapRemoveReject,case5");
    }
    
    if(chkvalue->age != 1) {
        TEST_FAIL("testMapRemoveReject,case6");
    }
    
    setEnvPort(++port);
    TEST_OK("testMapRemoveReject,case100");
    return 0;
}
