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
            case st(ArchiveMessage)::ApplyOpen: {
               if(msg->getFileName()->contains("testdata")) {
                    return ArchiveHandleResult::New(String::New("./tmp/testdata"),st(ArchiveHandleResult)::Type::Reject);
                } 
            }
            
            case st(ArchiveMessage)::ApplyUpload: {
                if(msg->getFileName()->contains("testdata")) {
                    return ArchiveHandleResult::New(String::New("./tmp/uploaddata"),st(ArchiveHandleResult)::Type::Reject);
                }
            } break;
            
            case st(ArchiveMessage)::ApplyDownload: {
                printf("apply download filename is %s \n",msg->getFileName()->toChars());
                if(msg->getFileName()->contains("testdata")) {
                    return ArchiveHandleResult::New(String::New("./tmp/testdata"),st(ArchiveHandleResult)::Type::Reject);
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
    
    File dir = File::New("./tmp/upload");
    dir->createDirs();
    
    auto option = ArchiveOption::New();
    option->setSavedPath(dir->getAbsolutePath());
    option->setHandler(MyHandler::New());
    
    ArchiveCenter center = ArchiveCenter::New(url,option);
    center->start();
    usleep(1000*100);
    
    ArchiveConnection c = ArchiveConnection::New(url);
    int ret1 = c->connect();

    File f = File::New("./tmp/testdata");
    int ret = c->upload(f);
    if(ret != -EPERM) {
        TEST_FAIL("testDocuement Deny case1,ret is %d",ret);
    }
    sleep(1);
    
    ArchiveConnection c2 = ArchiveConnection::New(url);
    c2->connect();
    int ret2 = c2->download("testdata","./tmp/abc");
    if(ret2 != -EPERM) {
        TEST_FAIL("testDocuement Deny case2,ret is %d",ret);
    }
    sleep(1);
    setEnvPort(++port);
    TEST_OK("testDocuement Deny case100");
    return 0;
}
