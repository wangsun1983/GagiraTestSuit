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
            case st(ArchiveMessage)::ApplyRename:
            case st(ArchiveMessage)::ApplyUpload: {
                if(msg->getFileName()->contains("testdata")) {
                    return ArchiveHandleResult(String::New("./tmp/testdata"));
                }
            } break;
            
            case st(ArchiveMessage)::ApplyDownload: {
                return ArchiveHandleResult(String::New("./tmp/testdata"));
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
        stream->open(O_APPEND);
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
    usleep(1000*100);
    ArchiveConnection c = ArchiveConnection::New(url);
    c->connect();
    
    File check_file = File::New("./tmp/testdata");
    if(!check_file->exists()) {
        TEST_FAIL("testDocuement Rename case1");
    }
    
    c->rename("testdata","abc_rename");
    if(check_file->exists()) {
        TEST_FAIL("testDocuement Rename case2");
    }
    
    File changeFile = File::New("./tmp/abc_rename");
    if(!changeFile->exists()) {
        TEST_FAIL("testDocuement Rename case3");
    }
    
    sleep(1);
    setEnvPort(++port);
    
    TEST_OK("testDocuement Rename case100");
    return 0;
}
