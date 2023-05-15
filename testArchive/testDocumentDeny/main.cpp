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
#include "DocumentConnection.hpp"
#include "DocumentCenter.hpp"

using namespace obotcha;
using namespace gagira;

DECLARE_CLASS(TaskInfo) IMPLEMENTS(Serializable){
public:
    String name;
    int value;
    DECLARE_REFLECT_FIELD(TaskInfo,name,value);
};

DECLARE_CLASS(MyHandler) IMPLEMENTS(DocumentHandler) {
public:
    String onUploadFile(String filename) {
        if(filename->contains("testdata")) {
            return st(DocumentMessage)::kReject;
        }
        
        return nullptr;
    }
    
    String onDownloadFile(String filename) {
        if(filename->equals("testdata")) {
            return st(DocumentMessage)::kReject;
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
        stream->open(st(OutputStream)::Append);
        String data = createString("");
        for(int i = 0;i < 1024;i++) {
          data = data->append(createString(st(System)::currentTimeMillis()));
        }
        stream->write(data->toByteArray());
        stream->close();
      }
    }
    
    printf("trace1 \n");
    auto option = createDocumentOption();
    option->setHandler(createMyHandler());
    
    DocumentCenter center = createDocumentCenter(url,option);
    center->start();
    usleep(1000*100);
    printf("trace2 \n");
    
    DocumentConnection c = createDocumentConnection(url);
    c->connect();
    File f = createFile("./tmp/testdata");
    int ret = c->upload(f);
    if(ret != -EACCES) {
        printf("upload ret is %d \n",ret);
    }
    sleep(1);
    
    DocumentConnection c2 = createDocumentConnection(url);
    c2->connect();
    int ret2 = c2->download("testdata","./tmp/abc");
    if(ret2 != -EACCES) {
        printf("download ret is %d \n",ret2);
    }
    sleep(1);
    printf("finish \n");
    setEnvPort(++port);
    sleep(1);
    return 0;
}
