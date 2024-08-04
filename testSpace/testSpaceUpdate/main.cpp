#include <stdio.h>
#include <unistd.h>

#include "Controller.hpp"
#include "Server.hpp"
#include "Inet4Address.hpp"
#include "ServletRequest.hpp"
#include "HttpResourceManager.hpp"
#include "Reflect.hpp"
#include "Utils.hpp"
#include "MqCenter.hpp"
#include "MqConnection.hpp"
#include "Serializable.hpp"
#include "CountDownLatch.hpp"
#include "TestLog.hpp"
#include "Handler.hpp"
#include "NetPort.hpp"
#include "Thread.hpp"
#include "SpaceConnection.hpp"
#include "SpaceCenter.hpp"

using namespace obotcha;
using namespace gagira;

CountDownLatch latch = createCountDownLatch(1024*32);

DECLARE_CLASS(StudentInfo) IMPLEMENTS(Serializable){
public:
    String name;
    int age;
    DECLARE_REFLECT_FIELD(StudentInfo,name,age);
};

int main() {
    int port = getEnvPort();
    String url = String::New("tcp://127.0.0.1:")->append(String::New(port));
    
    printf("trace1 \n");
    SpaceCenter center = createSpaceCenter(url,nullptr);
    center->start();
    usleep(1000*100);
    printf("trace2 \n");
    StudentInfo info = createStudentInfo();
    info->name = String::New("aawang");
    info->age = 123;
    printf("trace3 \n");
    SpaceConnection connection = createSpaceConnection(url);
    connection->connect();
    connection->update(String::New("Student"),info);
    printf("trace4 \n");
    auto result = connection->get<StudentInfo>(String::New("Student"));
    
    printf("name is %s,age is %d \n",result->name->toChars(),result->age);
    sleep(5);
    
    setEnvPort(++port);
    sleep(1);
    return 0;
}
