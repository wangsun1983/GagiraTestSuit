#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

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
            case st(ArchiveMessage)::ApplyDel:
            case st(ArchiveMessage)::ApplyOpen: {
               if(msg->getFileName()->contains("testdata")) {
                    return ArchiveHandleResult::New(String::New("./tmp/testdata"));
                } 
            }
            
            case st(ArchiveMessage)::ApplyUpload: {
                if(msg->getFileName()->contains("testdata")) {
                    return ArchiveHandleResult::New(String::New("./tmp/upload/uploaddata"));
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
      for(int i = 0;i<32;i++) {
        FileOutputStream stream = FileOutputStream::New(file);
        stream->open(O_APPEND);
        String data = String::New("");
        for(int i = 0;i < 32;i++) {
          data = data->append(String::New(st(System)::CurrentTimeMillis()));
        }
        stream->write(data->toByteArray());
        stream->close();
      }
    }
    
    File dir = File::New("./tmp/upload");
    dir->createDirs();

    //update uploadata    
    File upload_file = File::New("./tmp/upload/uploaddata");
    upload_file->createNewFile();
    FileOutputStream stream = FileOutputStream::New(upload_file);
    stream->open(O_APPEND);
    String data = String::New("");
    for(int i = 0;i < 32;i++) {
      data = data->append(String::New(st(System)::CurrentTimeMillis()));
    }
    stream->write(data->toByteArray());
    stream->close();


    auto option = ArchiveOption::New();
    option->setHandler(MyHandler::New());
    ArchiveCenter center = ArchiveCenter::New(url,option);
    center->start();
    
    ArchiveConnection c = ArchiveConnection::New(url);
    c->connect();
    int ret = c->upload(file);
    if(ret != -EEXIST) {
        TEST_FAIL("testDocuement upload same file case1");
    }
    setEnvPort(++port);
    TEST_OK("testDocuement upload same file case100");
    return 0;
}
