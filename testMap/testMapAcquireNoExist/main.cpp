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
    auto result = connection->get<StudentInfo>(String::New("abc"));
    if(result != nullptr) {
        TEST_FAIL("testMapAcquire no exists,case1");
    }
    
    setEnvPort(++port);
    TEST_OK("testMapAcquire no exists,case100");
    return 0;
}
