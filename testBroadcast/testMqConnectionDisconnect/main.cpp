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

using namespace obotcha;
using namespace gagira;

int gDisconnect = 0;
int gConnect = 0;

DECLARE_CLASS(ConnectionListener) IMPLEMENTS(BroadcastConnectionListener) {
public:
    int onMessage(String channel,ByteArray data) {
        return 1;
    }

    void onDisconnect() {
      gDisconnect = 100;
      //return false;
    }

    void onConnect() {
      gConnect = 200;
      //return false;
    }

    void onDetach(String channel) {
      //TODO
      //return false;
    }

     void onSustain(int code,String msg) {
        
     }
};

int main() {
    int port = getEnvPort();
    String url = String::New("tcp://127.0.0.1:")->append(String::New(port));

    int pid = fork();

    if(pid != 0) {
        sleep(1);
        BroadcastConnection connection = BroadcastConnection::New(url,ConnectionListener::New());
        connection->connect();
        sleep(10);
        if(gDisconnect != 100) {
          TEST_FAIL("testBroadcastConnectionDisconnect case1,disconnect is %d",gDisconnect);
        }

        if(gConnect != 200) {
          TEST_FAIL("testBroadcastConnectionConnect case2,connect is %d",gConnect);
        }

        TEST_OK("testBroadcastConnectionDisconnect case3");
    } else {
        DistributeCenterBuilder builder = DistributeCenterBuilder::New();
        builder->setUrl(url);
        BroadcastCenter center = builder->build();
        center->start();
        sleep(5);
        center->close();
        sleep(1);
        port++;
        setEnvPort(port);
    }

    return 0;
}
