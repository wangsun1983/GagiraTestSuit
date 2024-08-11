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
#include "ArchiveClient.hpp"
#include "ConcurrentQueue.hpp"

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
        stream->open(O_APPEND);
        String data = String::New("12345678910abcdefghijklmnopqrstuvwxyz");
        stream->write(data->toByteArray());
        stream->close();
    }
    
    auto option = ArchiveOption::New();
    option->setHandler(MyHandler::New());
    
    ArchiveCenter center = ArchiveCenter::New(url,option);
    center->start();
    usleep(1000*100);

    auto latch = CountDownLatch::New(1024);
    auto results = ConcurrentQueue<ByteArray>::New();

    for(int i = 0;i < 1024;i++) {
        auto thread = Thread::New([&]{
            auto connection = ArchiveConnection::New(url);
            connection->connect();
            FetchRet(fileno,ret) = connection->openStream(String::New("testdata"),st(IO)::ReadOnly);
            auto input = ArchiveInputStream::New(fileno,connection);

            ByteArray data = ByteArray::New(256);
            input->read(data);
            results->add(data);
            input->close();
            latch->countDown();
        });
        thread->start();
    }
    
    sleep(5);
    setEnvPort(++port);

    if(results->size() != 1024) {
        TEST_FAIL("testDocumentReadInMultiThread case1,size is %d",results->size());
    }
    
    for(int i = 0;i < 1024;i++) {
        auto data = results->get(i);
        if(!data->toString()->sameAs("12345678910abcdefghijklmnopqrstuvwxyz")) {
            TEST_FAIL("testDocumentReadInMultiThread case2,result is %s",data->toString()->toChars());
            return 0;
        }
    }
    
    TEST_OK("testDocumentReadInMultiThread case100");
    return 0;
}
