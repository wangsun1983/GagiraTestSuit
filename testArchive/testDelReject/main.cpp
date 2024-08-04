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
#include "IO.hpp"
#include "TestLog.hpp"

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
            case st(ArchiveMessage)::ApplyDel: {
                return st(ArchiveHandleResult)::DisAllow();
            }break;
            case st(ArchiveMessage)::ApplyOpen: {
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
                return ArchiveHandleResult::New(String::New("./tmp/testdata"));
            } break;
        }
        
        return nullptr;
    }
};

int main() {
    int port = getEnvPort();
    String url = String::New("tcp://127.0.0.1:")->append(String::New(port));
    //create file
    //prepare file
    File file = File::New("./tmp/testdata");
    if(!file->exists()) {
        file->createNewFile();
        FileOutputStream stream = FileOutputStream::New(file);
        stream->open(st(IO)::FileControlFlags::Append);
        String data = String::New("12345678910abcdefghijklmnopqrstuvwxyz");
        stream->write(data->toByteArray());
        stream->close();
    }
    
    if(!file->exists()) {
        TEST_FAIL("testDocuement Del Reject case1");
    }
    
    auto option = ArchiveOption::New();
    option->setHandler(MyHandler::New());
    
    ArchiveCenter center = ArchiveCenter::New(url,option);
    center->start();
    usleep(1000*100);
    
    ArchiveConnection c = ArchiveConnection::New(url);
    c->connect();
    
    usleep(1000*50);
    if(!file->exists()) {
        TEST_FAIL("testDocuement Del Reject case2");
    }
    int size = c->remove("testdata");

    sleep(1);
    setEnvPort(++port);
    if(!file->exists()) {
        TEST_FAIL("testDocuement Del Reject case3");
    }
    
    if(c->getErr() != -EPERM) {
        TEST_FAIL("testDocuement Del Reject case4");
    }
    TEST_OK("testDocuement Del Reject case100");
    return 0;
}
