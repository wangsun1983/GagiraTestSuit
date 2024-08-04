#include <stdio.h>
#include <unistd.h>

#include "Controller.hpp"
#include "Server.hpp"
#include "Inet4Address.hpp"
#include "ServletRequest.hpp"
#include "HttpResourceManager.hpp"
#include "Reflect.hpp"
#include "Utils.hpp"
#include "Serializable.hpp"
#include "CountDownLatch.hpp"
#include "TestLog.hpp"
#include "Handler.hpp"
#include "NetPort.hpp"
#include "Thread.hpp"
#include "ArchiveConnection.hpp"
#include "ArchiveCenter.hpp"
#include "ArchiveInputStream.hpp"
#include "ArchiveOutputStream.hpp"
#include "TestLog.hpp"
#include "Md.hpp"

using namespace obotcha;
using namespace gagira;

DECLARE_CLASS(TaskInfo) IMPLEMENTS(Serializable){
public:
    String name;
    int value;
    DECLARE_REFLECT_FIELD(TaskInfo,name,value);
};

DECLARE_CLASS(MyHandler) IMPLEMENTS(DistributeHandler) {
public:
    ArchiveHandleResult onRequest(DistributeLinker linker,ArchiveMessage msg) {
        switch(msg->getEvent()) {
            case st(ArchiveMessage)::ApplyOpen: {
               if(msg->getFileName()->contains("testdata")) {
                    return ArchiveHandleResult(String::New("./tmp/testdata"));
                } 
            }
            
            case st(ArchiveMessage)::ApplyUpload: {
                if(msg->getFileName()->contains("testdata")) {
                    return ArchiveHandleResult(String::New("./tmp/uploaddata"));
                }
            } break;
            
            case st(ArchiveMessage)::ApplyDownload: {
                return ArchiveHandleResult(String::New("./tmp/testdata"));
            } break;

            default: {
                printf("default path!!! \n");
                return ArchiveHandleResult(String::New("./tmp/")->append(msg->getFileName()));
            } break;
        }
        
        return nullptr;
    }
};

int main() {
    int port = getEnvPort();
    String url = String::New("tcp://127.0.0.1:")->append(String::New(port));
    
    auto option = ArchiveOption::New();
    option->setHandler(MyHandler::New());
    
    ArchiveCenter center = ArchiveCenter::New(url,option);
    center->start();
    usleep(1000*100);
    
    ArchiveConnection connection = ArchiveConnection::New(url);
    connection->connect();
    int ret = connection->rename(String::New("abc"),String::New("eeee"));
    if (ret == 0 || ret != -ENOENT) {
        TEST_FAIL("testDocuement rename file not exist case1");
    }

    setEnvPort(++port);
    
    TEST_OK("testDocuement rename file not exist case100");
    return 0;
}
