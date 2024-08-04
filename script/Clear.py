import os
import datetime
from enum import Enum


class TestType(Enum):
    TestNormal  = 1
    TestByGo   = 2
    TestByPython = 3 

testPath = [ "../testIo",
    "../testIo/testAsyncOutputChannel",
    "../testIo/testAsyncOutputChannelPool",
    "../testLang",
    "../testNet",
    "../testNet/testHttp",
    "../testNet/testHttp/testHttpConnection_go",
    "../testNet/testHttp/testHttpConnection_python3",
    #do not test"./testNet/testHttp/testHttpClient_python",
    "../testNet/testHttp/testHttpServer_go",
    "../testNet/testHttp/testHttpServer_python3",
    "../testNet/testHttp/testHttpRequestParser",
    "../testNet/testHttp/testHttpResponseWriter",
    #do not test "./testNet/testHttp2",
    "../testNet/testSocket/",
    "../testNet/testSocket/testSocketMonitor",
    "../testNet/testSocket/testDatagramSocket/Client",
    "../testNet/testSocket/testDatagramSocket/Server",
    "../testNet/testSocket/testLocalSocketImpl/Client",
    "../testNet/testSocket/testLocalSocketImpl/Server",
    "../testNet/testSocket/testSocketMonitor/",
    "../testNet/testSocket/testSocksSocketImpl/Client",
    "../testNet/testSocket/testSocksSocketImpl/Server",
    "../testNet/testSocket/testSocksSocketImpl/Server_go",
    "../testNet/testSocket/testSSLSocketImpl/Server",
    "../testNet/testSocketV6/testDatagramSocket/Client",
    "../testNet/testSocketV6/testDatagramSocket/Server",
    "../testNet/testSocketV6/testSocksSocketImpl/Client",
    "../testNet/testSocketV6/testSocksSocketImpl/Server",
    "../testNet/testWebsocket/testWebSocketClient/python_server",
    "../testNet/testWebsocket",
    "../testNet/testWebsocket/testWebSocketServer/go_client",
    "../testNet/testWebsocket/testWebSocketServer/python_client",
    "../testProcess",
    "../testSecurity",
    "../testUtil",
    "../testUtil/testText",
    "../testUtil/testFilament",
    "../testUtil/testConcurrent",
    "../testSql",
    "../testSql/testMySqlClient",
    "../testSql/testRedisClient",
    "../testSql/testSqlite3Client",]

#create report again
REPORT_DIR = '../Report'
BUILD_REPORT_DIR ='../Report/BuildReport'
EXECUTE_REPORT_DIR ='../Report/ExecuteReport'

def prepare(path):
    print("prepare path is " + path)
    os.popen("rm -rf " + path + "tmp")
    os.popen("mkdir " + path + "tmp")

#define defaultTest
def scanTest(path):
    isMakefileExist = False
    #judge whether path is a folder
    if os.path.isfile(path) or not os.path.exists(path):
        return

    for filename in os.listdir(path):
        if filename == "mytest":
            os.popen("cd " + path + " && rm mytest")
        if filename == "tmp":
            os.popen("cd " + path + " && rm -rf tmp")
        if filename == "server":
            os.popen("cd " + path + " && rm -rf server")
        if filename == "client":
            os.popen("cd " + path + " && rm -rf client")

def main():
    #remove report
    for path in testPath:
        for filename in os.listdir(path):
            testFolder = path + "/" + filename + "/";
            scanTest(testFolder)


if __name__ == '__main__':
    main()