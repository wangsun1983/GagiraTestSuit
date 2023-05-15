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
        printf("msg->event is %d \n",msg->event);
        switch(msg->event) {
            case st(ArchiveMessage)::ApplyRename:
            case st(ArchiveMessage)::ApplyUpload: {
                printf("upload!!!!! trace1 \n");
                if(msg->filename->contains("testdata")) {
                    printf("upload!!!!! trace2 \n");
                    return ArchiveHandleResult(createString("./tmp/testdata"));
                }
            } break;
            
            case st(ArchiveMessage)::ApplyDownload: {
                printf("download!!!!! trace1 \n");
                return ArchiveHandleResult(createString("./tmp/testdata"));
            } break;
        }
        
        return nullptr;
    }
};

int main() {
    int port = getEnvPort();
    String url = createString("tcp://127.0.0.1:")->append(createString(port));
    
    //create file
    //prepare file
    File file = createFile("./tmp/testdata");
    if(!file->exists()) {
      file->createNewFile();
        for(int i = 0;i<1024;i++) {
        FileOutputStream stream = createFileOutputStream(file);
        stream->open(O_APPEND);
        String data = createString("");
        for(int i = 0;i < 1024;i++) {
          data = data->append(createString(st(System)::currentTimeMillis()));
        }
        stream->write(data->toByteArray());
        stream->close();
      }
    }
    
    printf("trace1 \n");
    auto option = createArchiveOption();
    option->setHandler(createMyHandler());
    
    ArchiveCenter center = createArchiveCenter(url,option);
    center->start();
    usleep(1000*100);
    printf("trace2 \n");
    ArchiveConnection c = createArchiveConnection(url);
    c->connect();
    //c->rename("testdata","abc_rename");
    sleep(1);
    setEnvPort(++port);
    sleep(1);
    return 0;
}
