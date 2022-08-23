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

using namespace obotcha;
using namespace gagira;

DECLARE_CLASS(StudentInfo) IMPLEMENTS(Serializable){
public:
    String name;
    int age;
    DECLARE_REFLECT_FIELD(StudentInfo,name,age);
};

DECLARE_CLASS(ConnectionListener) IMPLEMENTS(MqConnectionListener) {
public:
    bool onEvent(String channel,ByteArray data) {
        StudentInfo info = createStudentInfo();
        info->deserialize(data);
        printf("name is %s,age is %d \n",info->name->toChars(),info->age);
        //for(int i = 0;i < data->size();i++) {
        //    printf("data[%d] is %x \n",i,data[i]);
        //}
        return true;
    }
};


int main() {
    
    MqConnection connection = createMqConnection("tcp://127.0.0.1:1250");
    connection->connect();

    StudentInfo student = createStudentInfo();
    student->name = createString("wangsl");
    student->age = 12;

    connection->subscribe("info",createConnectionListener());
    printf("start send \n");
    connection->publish("info",student,st(MqMessage)::PublishOneShot|st(MqMessage)::Acknowledge);
    while(1) {
        sleep(100);
    }

    return 0;
}

