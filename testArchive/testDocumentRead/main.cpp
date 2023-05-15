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
            case st(ArchiveMessage)::ApplyOpen: {
               if(msg->filename->contains("testdata")) {
                    printf("upload!!!!! trace2 \n");
                    return ArchiveHandleResult(createString("./tmp/testdata"));
                } 
            }
            
            case st(ArchiveMessage)::ApplyUpload: {
                printf("upload!!!!! trace1 \n");
                if(msg->filename->contains("testdata")) {
                    printf("upload!!!!! trace2 \n");
                    return ArchiveHandleResult(createString("./tmp/uploaddata"));
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
        FileOutputStream stream = createFileOutputStream(file);
        stream->open(st(OutputStream)::Append);
        String data = createString("12345678910abcdefghijklmnopqrstuvwxyz");
        stream->write(data->toByteArray());
        stream->close();
    }
    
    printf("trace1 \n");
    auto option = createArchiveOption();
    option->setHandler(createMyHandler());
    
    ArchiveCenter center = createArchiveCenter(url,option);
    center->start();
    usleep(1000*100);
    
    ByteArray data = createByteArray(4);
    auto input = createArchiveInputStream("testdata",url);
    if(!input->open()) {
        printf("stream open fail \n");
        return 1;
    }
    printf("input seek to \n");
    input->seekTo(4);
    printf("input trace2 \n");
    auto len = input->read(data);
    printf("len %d ,data is %s\n",len,data->toString()->toChars());
    
    input->close();
    printf("finish close \n");
    sleep(1);
    setEnvPort(++port);
    sleep(1);
    return 0;
}
