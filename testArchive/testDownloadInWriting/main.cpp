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
#include "TestLog.hpp"
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
            case st(ArchiveMessage)::ApplyOpen: {
               if(msg->getFileName()->contains("testdata")) {
                    return ArchiveHandleResult(String::New("./tmp/testdata"));
                } 
            }
            
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
    
    auto option = ArchiveOption::New();
    option->setHandler(MyHandler::New());
    
    ArchiveCenter center = ArchiveCenter::New(url,option);
    center->start();
    usleep(1000*100);
    
    ArchiveConnection connection = ArchiveConnection::New(url);
    connection->connect();
    FetchRet(fileno,ret) = connection->openStream(String::New("testdata"),O_WRONLY|O_APPEND);
    auto output = ArchiveOutputStream::New(fileno,connection);
    
    ArchiveConnection connection2 = ArchiveConnection::New(url);
    connection2->connect();
    ret = connection2->download(String::New("testdata"),String::New("./tmp/downloadfile"));
    if(ret == 0) {
        TEST_FAIL("testDocuement download in Writing case1");
    }

    sleep(1);
    output->close();
    ret = connection2->download(String::New("testdata"),String::New("./tmp/downloadfile"));
    if(ret != 0) {
        TEST_FAIL("testDocuement download in Writing case2");
    }


    File f = File::New("./tmp/downloadfile");
    if(!f->exists()) {
        TEST_FAIL("testDocuement download in Writing case3");
        return 0;
    }

    Md md5sum = Md::New();

    String str1 = md5sum->encodeFile(File::New("./tmp/downloadfile"));
    String str2 = md5sum->encodeFile(File::New("./tmp/testdata"));

    if(!str1->sameAs(str2)) {
        TEST_FAIL("testDocuement download in Writing case4");
    }

    setEnvPort(++port);
    
    TEST_OK("testDocuement download in Writing case100");
    return 0;
}
