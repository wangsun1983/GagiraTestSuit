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

int main() {
    File file = File::New("./tmp/testdata");
    if(!file->exists()) {
        file->createNewFile();
        FileOutputStream stream = FileOutputStream::New(file);
        stream->open(O_APPEND);
        String data = String::New("12345678910abcdefghijklmnopqrstuvwxyz");
        stream->write(data->toByteArray());
        stream->close();
    }

    //ApplyUploadConnectMessage
    while(1) {
        ApplyUploadConnectMessage msg = ApplyUploadConnectMessage::New();
        if(msg->getEvent() != st(ArchiveMessage)::ApplyUploadConnect) {
            TEST_FAIL("testArchiveMessage case1");
        }
        break;
    }

    //ConfirmUploadConnectMessage
    while(1) {
        ConfirmUploadConnectMessage msg = ConfirmUploadConnectMessage::New();
        if(msg->getEvent() != st(ArchiveMessage)::ConfirmUploadConnect) {
            TEST_FAIL("testArchiveMessage case2");
        }

        ConfirmUploadConnectMessage msg2= ConfirmUploadConnectMessage::New(100);
        if(msg2->getPort() != 100) {
            TEST_FAIL("testArchiveMessage case3");
        }
        break;
    }

    //ApplyUploadMessage
    while(1) {
        ApplyUploadMessage msg = ApplyUploadMessage::New();
        if(msg->getEvent() != st(ArchiveMessage)::ApplyUpload) {
            TEST_FAIL("testArchiveMessage case4");
        }

        File file = File::New("./tmp/testdata");
        ApplyUploadMessage msg2 = ApplyUploadMessage::New(file);
        if(msg2->getUploadFileLength() != file->length()) {
            TEST_FAIL("testArchiveMessage case5");
        }

        if(!msg2->getFileName()->sameAs("testdata")) {
            TEST_FAIL("testArchiveMessage case6");
        }
        break;
    }

    //ConfirmApplyUploadMessage
    while(1) {
        ConfirmApplyUploadMessage msg = ConfirmApplyUploadMessage::New();
        if(msg->getEvent() != st(ArchiveMessage)::ConfirmUpload) {
            TEST_FAIL("testArchiveMessage case7");
        }

        ConfirmApplyUploadMessage msg2 = ConfirmApplyUploadMessage::New(123);
        if(msg2->getEvent() != st(ArchiveMessage)::ConfirmUpload) {
            TEST_FAIL("testArchiveMessage case8");
        }

        if(msg2->getPermitFlag() != 123) {
            TEST_FAIL("testArchiveMessage case9");
        }
        break;
    }

    //ApplyDownloadMessage
    while(1) {
        ApplyDownloadMessage msg = ApplyDownloadMessage::New();
        if(msg->getEvent() != st(ArchiveMessage)::ApplyDownload) {
            TEST_FAIL("testArchiveMessage case10");
        }

        ApplyDownloadMessage msg2 = ApplyDownloadMessage::New(String::New("abcd"));
        if(msg2->getEvent() != st(ArchiveMessage)::ApplyDownload) {
            TEST_FAIL("testArchiveMessage case11");
        }

        if(!msg2->getFileName()->sameAs("abcd")) {
            TEST_FAIL("testArchiveMessage case12");
        }
        break;
    }

    //ConfirmDownloadMessage
    while(1) {
        ConfirmDownloadMessage msg = ConfirmDownloadMessage::New();
        if(msg->getEvent() != st(ArchiveMessage)::ConfirmDownload) {
            TEST_FAIL("testArchiveMessage case13");
        }

        ConfirmDownloadMessage msg2 = ConfirmDownloadMessage::New(1122);
        if(msg2->getEvent() != st(ArchiveMessage)::ConfirmDownload) {
            TEST_FAIL("testArchiveMessage case14");
        }

        if(msg2->getPermitFlag() != 1122) {
            TEST_FAIL("testArchiveMessage case15");
        }

        ConfirmDownloadMessage msg3 = ConfirmDownloadMessage::New(File::New("./tmp/testdata"),123);
        if(msg3->getEvent() != st(ArchiveMessage)::ConfirmDownload) {
            TEST_FAIL("testArchiveMessage case14");
        }

        if(!msg3->getFileName()->sameAs("testdata")) {
            TEST_FAIL("testArchiveMessage case15");
        }

        if(msg3->getFileNo() != 123) {
            TEST_FAIL("testArchiveMessage case16");
        }
        break;
    }

    //ProcessDownloadMessage
    while(1) {
        ProcessDownloadMessage msg = ProcessDownloadMessage::New(123);
        if(msg->getEvent() != st(ArchiveMessage)::ProcessDownload) {
            TEST_FAIL("testArchiveMessage case17");
        }

        if(msg->getFileNo() != 123) {
            TEST_FAIL("testArchiveMessage case18");
        }
        break;
    }

    //QueryInfoMessage
    while(1) {
        QueryInfoMessage msg = QueryInfoMessage::New(String::New("abc"));
        if(msg->getEvent() != st(ArchiveMessage)::QueryInfo) {
            TEST_FAIL("testArchiveMessage case19");
        }

        if(!msg->getFileName()->sameAs("abc")) {
            TEST_FAIL("testArchiveMessage case20");
        }
        break;
    }

    //ConfirmQueryInfoMessage
    while(1) {
        ConfirmQueryInfoMessage msg = ConfirmQueryInfoMessage::New();
        if(msg->getEvent() != st(ArchiveMessage)::ConfirmQueryInfo) {
            TEST_FAIL("testArchiveMessage case21");
        }

        ConfirmQueryInfoMessage msg2 = ConfirmQueryInfoMessage::New(123);
        if(msg2->getEvent() != st(ArchiveMessage)::ConfirmQueryInfo) {
            TEST_FAIL("testArchiveMessage case22");
        }

        if(msg2->getPermitFlag() != 123) {
            TEST_FAIL("testArchiveMessage case23");
        }

        ConfirmQueryInfoMessage msg3 = ConfirmQueryInfoMessage::New(File::New("./tmp/testdata"));
        if(msg3->getEvent() != st(ArchiveMessage)::ConfirmQueryInfo) {
            TEST_FAIL("testArchiveMessage case24");
        }

        File file = File::New("./tmp/testdata");
        if(msg3->getQueryFileSize() != file->length()) {
            TEST_FAIL("testArchiveMessage case25");
        }

        if(!msg3->isPermitted()) {
            TEST_FAIL("testArchiveMessage case26");
        }
        break;
    }

    //ApplyOpenMessage
    while(1) {
        ApplyOpenMessage msg2 = ApplyOpenMessage::New(String::New("aabbcc"),1233);
        if(msg2->getEvent() != st(ArchiveMessage)::ApplyOpen) {
            TEST_FAIL("testArchiveMessage case28");
        }

        if(!msg2->getFileName()->sameAs("aabbcc")) {
            TEST_FAIL("testArchiveMessage case29");
        }

        if(msg2->getFlags() != 1233) {
            TEST_FAIL("testArchiveMessage case30");
        }
        break;
    }

    //ConfirmOpenMessage
    while(1) {
        ConfirmOpenMessage msg = ConfirmOpenMessage::New();
        if(msg->getEvent() != st(ArchiveMessage)::ConfirmOpen) {
            TEST_FAIL("testArchiveMessage case31");
        }

        ConfirmOpenMessage msg2 = ConfirmOpenMessage::New(111,222);
        if(msg2->getEvent() != st(ArchiveMessage)::ConfirmOpen) {
            TEST_FAIL("testArchiveMessage case32");
        }

        if(msg2->getFileNo() != 111) {
            TEST_FAIL("testArchiveMessage case33");
        }

        if(msg2->getPermitFlag() != 222) {
            TEST_FAIL("testArchiveMessage case34");
        }
        break;
    }

    //ApplySeekToMessage
    while(1) {
        ApplySeekToMessage msg = ApplySeekToMessage::New(111,222,st(ApplySeekToMessage)::Read);
        if(msg->getEvent() != st(ArchiveMessage)::ApplySeekTo) {
            TEST_FAIL("testArchiveMessage case35");
        }

        if(msg->getFileNo() != 111) {
            TEST_FAIL("testArchiveMessage case36");
        }

        if(msg->getStartPos() != 222) {
            TEST_FAIL("testArchiveMessage case37");
        }

        if(msg->getSeekType() != st(ApplySeekToMessage)::Read) {
            TEST_FAIL("testArchiveMessage case38");
        }

        ApplySeekToMessage msg2 = ApplySeekToMessage::New(111,222,st(ApplySeekToMessage)::Write);
        if(msg2->getSeekType() != st(ApplySeekToMessage)::Write) {
            TEST_FAIL("testArchiveMessage case38_1,type is %d",msg2->getSeekType());
        }
        break;
    }

    //ConfirmSeekToMessage
    while(1) {
        ConfirmSeekToMessage msg = ConfirmSeekToMessage::New();
        if(msg->getEvent() != st(ArchiveMessage)::ConfirmSeekTo) {
            TEST_FAIL("testArchiveMessage case39");
        }

        ConfirmSeekToMessage msg2 = ConfirmSeekToMessage::New(1122);
        if(msg2->getEvent() != st(ArchiveMessage)::ConfirmSeekTo) {
            TEST_FAIL("testArchiveMessage case40");
        }

        if(msg2->getPermitFlag() != 1122) {
            TEST_FAIL("testArchiveMessage case41");
        }
        break;
    }

    //ApplyReadMessage
    while(1) {
        ApplyReadMessage msg = ApplyReadMessage::New(11,22);
        if(msg->getEvent() != st(ArchiveMessage)::ProcessRead) {
            TEST_FAIL("testArchiveMessage case42");
        }

        if(msg->getFileNo() != 11) {
            TEST_FAIL("testArchiveMessage case43");
        }

        if(msg->getReadLength() != 22) {
            TEST_FAIL("testArchiveMessage case44");
        }
        break;
    }

    //ConfirmReadMessage
    while(1) {
        ConfirmReadMessage msg = ConfirmReadMessage::New();
        if(msg->getEvent() != st(ArchiveMessage)::ConfirmRead) {
            TEST_FAIL("testArchiveMessage case45");
        }

        String data = String("hello");
        ConfirmReadMessage msg2 = ConfirmReadMessage::New(data->toByteArray());
        if(msg2->getEvent() != st(ArchiveMessage)::ConfirmRead) {
            TEST_FAIL("testArchiveMessage case46");
        }

        String response = msg2->getData()->toString();
        if(!response->sameAs(data)) {
            TEST_FAIL("testArchiveMessage case47");
        }

        if(msg2->getReadLength() != data->size()) {
            TEST_FAIL("testArchiveMessage case48,msg2->getReadLength() is %d,data->size() is %d",msg2->getReadLength(),data->size());
        }

        ConfirmReadMessage msg3 = ConfirmReadMessage::New(1234);
        if(msg3->getEvent() != st(ArchiveMessage)::ConfirmRead) {
            TEST_FAIL("testArchiveMessage case49");
        }

        if(msg3->getPermitFlag() != 1234) {
            TEST_FAIL("testArchiveMessage case50");
        }
        break;
    }

    //ApplyWriteMessage
    while(1) {
        String data = String::New("abcd");
        ApplyWriteMessage msg = ApplyWriteMessage::New(123,data->toByteArray());
        if(msg->getEvent() != st(ArchiveMessage)::ApplyWrite) {
            TEST_FAIL("testArchiveMessage case51");
        }

        String response = msg->getData()->toString();
        if(!data->sameAs(response)) {
            TEST_FAIL("testArchiveMessage case52");
        }

        if(msg->getFileNo() != 123) {
            TEST_FAIL("testArchiveMessage case53");
        }
        break;
    }

    //ConfirmWriteMessage
    while(1) {
        ConfirmWriteMessage msg = ConfirmWriteMessage::New();
        if(msg->getEvent() != st(ArchiveMessage)::ConfirmWrite) {
            TEST_FAIL("testArchiveMessage case54");
        }

        ConfirmWriteMessage msg2 = ConfirmWriteMessage::New(123);
        if(msg2->getEvent() != st(ArchiveMessage)::ConfirmWrite) {
            TEST_FAIL("testArchiveMessage case55");
        }

        if(msg2->getPermitFlag() != 123) {
            TEST_FAIL("testArchiveMessage case56");
        }
        break;
    }

    //ApplyDelMessage
    while(1) {
        ApplyDelMessage msg = ApplyDelMessage::New(String::New("1234"));
        if(msg->getEvent() != st(ArchiveMessage)::ApplyDel) {
            TEST_FAIL("testArchiveMessage case57");
        }

        if(!msg->getFileName()->sameAs("1234")) {
            TEST_FAIL("testArchiveMessage case58");
        }
        break;
    }

    //ConfirmDelMessage
    while(1) {
        ConfirmDelMessage msg = ConfirmDelMessage::New();
        if(msg->getEvent() != st(ArchiveMessage)::ConfirmDel) {
            TEST_FAIL("testArchiveMessage case59");
        }

        ConfirmDelMessage msg2 = ConfirmDelMessage::New(1122);
        if(msg2->getEvent() != st(ArchiveMessage)::ConfirmDel) {
            TEST_FAIL("testArchiveMessage case60");
        }

        if(msg2->getPermitFlag() != 1122) {
            TEST_FAIL("testArchiveMessage case61,msg->getPermitFlag() is %d",msg->getPermitFlag());
        }
        break;
    }

    //ApplyRenameMessage
    while(1) {
        ApplyRenameMessage msg = ApplyRenameMessage::New(String::New("ori"),String::New("new"));
        if(msg->getEvent() != st(ArchiveMessage)::ApplyRename) {
            TEST_FAIL("testArchiveMessage case62");
        }

        if(!msg->getRenameOriginalName()->sameAs("ori")) {
            TEST_FAIL("testArchiveMessage case63");
        }

        if(!msg->getRenameNewName()->sameAs("new")) {
            TEST_FAIL("testArchiveMessage case64");
        }
        break;
    }

    //ConfirmRenameMessage
    while(1) {
        ConfirmRenameMessage msg = ConfirmRenameMessage::New();
        if(msg->getEvent() != st(ArchiveMessage)::ConfirmRename) {
            TEST_FAIL("testArchiveMessage case65");
        }

        ConfirmRenameMessage msg2 = ConfirmRenameMessage::New(1233);
        if(msg2->getEvent() != st(ArchiveMessage)::ConfirmRename) {
            TEST_FAIL("testArchiveMessage case66");
        }

        if(msg2->getPermitFlag() != 1233) {
            TEST_FAIL("testArchiveMessage case67");
        }
        break;
    }

    //ApplyCloseStreamMessage
    while(1) {
        ApplyCloseStreamMessage msg = ApplyCloseStreamMessage::New(123);
        if(msg->getEvent() != st(ArchiveMessage)::ApplyCloseStream) {
            TEST_FAIL("testArchiveMessage case68");
        }

        if(msg->getFileNo() != 123) {
            TEST_FAIL("testArchiveMessage case69");
        }
        break;
    }

    //ConfirmCloseStreamMessage
    while(1) {
        ConfirmCloseStreamMessage msg = ConfirmCloseStreamMessage::New();
        if(msg->getEvent() != st(ArchiveMessage)::ConfirmCloseStream) {
            TEST_FAIL("testArchiveMessage case70");
        }

        ConfirmCloseStreamMessage msg2 = ConfirmCloseStreamMessage::New(122);
        if(msg2->getEvent() != st(ArchiveMessage)::ConfirmCloseStream) {
            TEST_FAIL("testArchiveMessage case71");
        }

        if(msg2->getPermitFlag() != 122) {
            TEST_FAIL("testArchiveMessage case72");
        }
        break;
    }
 
    TEST_OK("testArchiveMessage case100");
    return 0;
}
