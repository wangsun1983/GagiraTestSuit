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
                    return ArchiveHandleResult(String::New("./tmp/uploaddata"));
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
    printf("trace1 \n");
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
    
    File dir = File::New("./tmp/download/");
    dir->createDirs();

    auto option = ArchiveOption::New();
    option->setHandler(MyHandler::New());
    printf("trace2 \n");
    ArchiveCenter center = ArchiveCenter::New(url,option);
    center->start();
    usleep(1000*100);
    for(int i = 0;i < 10;i++) {
        Thread t = Thread::New([&url](int count){
            String filename = String::New("./tmp/download/file")->append(Integer::New(count)->toString());
            ArchiveConnection c = ArchiveConnection::New(url);
            c->connect();
            c->download(String::New("testdata"),filename);
        },i);
        t->start();
    }
    sleep(5);
    printf("trace3 \n");
    Md md = Md::New();
    String str2 = md->encodeFile(File::New("./tmp/testdata"));
    for(int i = 0;i < 10;i++) {
        String filename = String::New("./tmp/download/file")->append(Integer::New(i)->toString());
        String str1 = md->encodeFile(File::New(filename));
        if(!str2->sameAs(str1)) {
            TEST_FAIL("testDocuement download multi thread case1");
        }
    }
    printf("trace4 \n");
    setEnvPort(++port);
    sleep(1);
    TEST_OK("testDocuement download multi thread case100");
    printf("trace5 \n");
    return 0;
}
