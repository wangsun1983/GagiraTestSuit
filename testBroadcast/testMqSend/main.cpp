#include <stdio.h>
#include <unistd.h>

#include "Controller.hpp"
#include "Server.hpp"
#include "Inet4Address.hpp"
#include "ServletRequest.hpp"
#include "HttpResourceManager.hpp"
#include "Reflect.hpp"
#include "Utils.hpp"
#include "BroadcastCenter.hpp"
#include "BroadcastConnection.hpp"
#include "Serializable.hpp"
#include "CountDownLatch.hpp"
#include "TestLog.hpp"
#include "DistributeCenterBuilder.hpp"
#include "Handler.hpp"
#include "NetPort.hpp"
#include "Thread.hpp"

using namespace obotcha;
using namespace gagira;

CountDownLatch latch = CountDownLatch::New(1024*32);

DECLARE_CLASS(StudentInfo) IMPLEMENTS(Serializable){
public:
    String name;
    int age;
    DECLARE_REFLECT_FIELD(StudentInfo,name,age);
};

DECLARE_CLASS(ConnectionListener) IMPLEMENTS(BroadcastConnectionListener) {
public:
    int onMessage(String channel,ByteArray data) {
        StudentInfo info = StudentInfo::New();
        info->deserialize(data);
        if(!info->name->sameAs("wang") && info->age != 12) {
            TEST_FAIL("testmqsend case1,name is %s,age is %d",info->name->toChars(),info->age);
        }
        latch->countDown();
        return 0;
    }

    void onDisconnect() {
      //TODO
      //return false;
    }

    void onConnect() {
      //TODO
      //return false;
    }

    void onDetach(String channel) {
      //TODO
      //return false;
    }

    void onSustain(int code,String msg){}
};

DECLARE_CLASS(MyHandler) IMPLEMENTS (Handler) {
public:
    _MyHandler(CountDownLatch l) {
        latch = l;
    }

    void handleMessage(Message msg) {
        printf("latch count is %d \n",latch->getCount());
        this->sendEmptyMessageDelayed(1,1*1000);
    }

private:
    CountDownLatch latch;
};


int main() {
    int port = getEnvPort();
    String url = String::New("tcp://127.0.0.1:")->append(String::New(port));
    port++;
    
    Thread t1 = Thread::New([&url] {
        sleep(1);
        BroadcastConnection connection = BroadcastConnection::New(url,ConnectionListener::New());
        connection->connect();
        connection->subscribeChannel("info");
        MyHandler h = MyHandler::New(latch);
        h->sendEmptyMessageDelayed(1,1*1000);
        latch->await();
        TEST_OK("testmqsend case100");
    });
        
    Thread t2 = Thread::New([&url] {
        DistributeCenterBuilder builder = DistributeCenterBuilder::New();
        builder->setUrl(url);
        BroadcastCenter center = builder->buildBroadcastCenter();
        int ret = center->start();
        printf("mqsend ret is %d \n",ret);
        BroadcastConnection connection = BroadcastConnection::New(url);
        connection->connect();
        //start send
        sleep(2);
        for(int i = 0; i <1024*32;i++) {
            StudentInfo student = StudentInfo::New();
            student->name = String::New("wang");
            student->age = 12;
            connection->publishMessage("info",student,
                BroadcastMessageParam::New()->setFlags(st(BroadcastMessage)::OneShotFlag)->build());
        }
        sleep(5);
    });
    
    t1->start();
    t2->start();
    
    t1->join();
    t2->join();
    
    setEnvPort(++port);
    sleep(1);
    return 0;
}
