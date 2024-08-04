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
            case st(ArchiveMessage)::ApplyUpload: {
                if(msg->getFileName()->contains("testdata")) {
                    String finalPath = String::New("./tmp/upload/uploaddata")->append(Integer::New(count)->toString());
                    count++;
                    return ArchiveHandleResult(String::New(finalPath));
                }
            } break;
            
            case st(ArchiveMessage)::ApplyDownload: {
                return ArchiveHandleResult(String::New("./tmp/testdata"));
            } break;
        }
        
        return nullptr;
    }
private:
    int count = 0;
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

    File uploadDir = File::New("./tmp/upload");
    uploadDir->createDirs();
    
    ArchiveCenter center = ArchiveCenter::New(url,option);
    center->start();
    usleep(1000*100);
    
    for(int i = 0; i<10;i++) {
        Thread t = Thread::New([&url]{
            ArchiveConnection c = ArchiveConnection::New(url);
            c->connect();
            File f = File::New("./tmp/testdata");
            c->upload(f);
        });
        t->start();
    }
    usleep(1000*5000);
    setEnvPort(++port);
    
    File uploadFile = File::New("./tmp/upload/uploaddata");
    File srcfile = File::New("./tmp/testdata");
    Md mdsum = Md::New();
    auto str2 = mdsum->encodeFile(srcfile);
    for(int i = 0;i < 10 ;i++) {
        String finalPath = String::New("./tmp/upload/uploaddata")->append(Integer::New(i)->toString());
        File file = File::New(finalPath);
        auto str1 = mdsum->encodeFile(file);
        if (!str1->sameAs(str2)) {
            TEST_FAIL("testDocuement upload multithread case1");
        }    
    }
    
    TEST_OK("testDocuement upload upload multithread case100");

    return 0;
}
