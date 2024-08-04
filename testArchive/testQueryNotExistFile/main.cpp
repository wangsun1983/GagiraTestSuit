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
            case st(ArchiveMessage)::ApplyDel:
            case st(ArchiveMessage)::ApplyOpen:
            case st(ArchiveMessage)::QueryInfo: {
               if(msg->getFileName()->contains("testdata")) {
                    return ArchiveHandleResult::New(String::New("./tmp/testdata"));
                } 
            }
            
            case st(ArchiveMessage)::ApplyUpload: {
                if(msg->getFileName()->contains("testdata")) {
                    return ArchiveHandleResult::New(String::New("./tmp/uploaddata"));
                }
            } break;
            
            case st(ArchiveMessage)::ApplyDownload: {
                if(msg->getFileName()->sameAs("testdata")) {
                    return ArchiveHandleResult::New(String::New("./tmp/testdata"));
                }
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
    
    ArchiveConnection c = ArchiveConnection::New(url);
    c->connect();
    int size = c->querySize("testdata");
    if(size != 0) {
        TEST_FAIL("testDocuement Query file not exist case1,size is %d,real size is %d",size);
    }
    
    sleep(1);
    setEnvPort(++port);
    TEST_OK("testDocuement Query file not exist case100");
    return 0;
}
