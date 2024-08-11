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
    
    //create file
    //prepare file
    File file = File::New("./tmp/testdata");
    if(!file->exists()) {
      file->createNewFile();
        for(int i = 0;i<1024;i++) {
        FileOutputStream stream = FileOutputStream::New(file);
        stream->open(st(IO)::FileControlFlags::Append);
        String data = String::New("");
        for(int i = 0;i < 1024;i++) {
          data = data->append(String::New(st(System)::CurrentTimeMillis()));
        }
        stream->write(data->toByteArray());
        stream->close();
      }
    }
    
    auto option = ArchiveOption::New();
    option->setHandler(MyHandler::New());
    
    ArchiveCenter center = ArchiveCenter::New(url,option);
    center->start();
    
    ArchiveConnection c = ArchiveConnection::New(url);
    c->connect();
    usleep(50*1000);
    c->close();
    
    int size = c->querySize("testdata");
    if(size != 0) {
        TEST_FAIL("testDocuement Query after close case1,size is %d,real size is %d",size,file->length());
    }
    
    auto err = c->getErr();
    if (err != -EINVAL) {
        TEST_FAIL("testDocuement Query after close case2,err is %d",err);
    }
    
    sleep(1);
    setEnvPort(++port);
    TEST_OK("testDocuement Query after close case100");
    return 0;
}
