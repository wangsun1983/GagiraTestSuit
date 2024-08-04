import os
import datetime
import threading
import time
from enum import Enum

#prepare
#pip3 install websocket-client

class TestType(Enum):
    TestNormal             = 1
    TestRunGoClient        = 2
    TestRunGoServer        = 3
    TestRunPythonClient    = 4
    TestRunPythonServer    = 5

testPath = [
    "../testIo",
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
    "../testNet/testSocket",
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
    "../testSql/testSqlite3Client",
    #"../testSql/testMySqlClient",
    #"../testSql/testRedisClient",
    #"../testSql/testSqlite3Client"
    ]

#create report again
REPORT_DIR = '../Report'
BUILD_REPORT_DIR ='../Report/BuildReport'
EXECUTE_REPORT_DIR ='../Report/ExecuteReport'

def prepare(path):
    print("prepare path is " + path)
    os.popen("rm -rf " + path + "tmp").read()
    while os.path.exists(path + "tmp"):
        time.sleep(0.1)
        print("path " + path + "tmp already exist,wait....")
    os.popen("mkdir " + path + "tmp").read()
    #remove mytest,client,server

def waitTestData(path):
    filesize = 0
    print("waitTestData path is " + path)
    if os.path.exists(path + "tmp/testdata"):
        print("file exist!!!!")
        while True:
            print("wait createData " + path);
            filesize = os.path.getsize(path + "tmp/testdata")
            time.sleep(0.1)
            if filesize == os.path.getsize(path + "tmp/testdata"):
                return

execThreadLog = ""
def execByThread(cmd,readlog):
    print("execByThread cmd is ",cmd)
    global execThreadLog
    if readlog:
        execThreadLog = os.popen(cmd).read()
    else:
        print("execByThread trace ",cmd)
        ret = os.popen(cmd).read()
        print("execByThread ret ",ret)

#define defaultTest
def scanTest(path):
    print("path is ",path)

    isMakefileExist = False
    isPrepareFileExist = False
    #judge whether path is a folder
    if os.path.isfile(path) or not os.path.exists(path):
        print("path trace is ",path)
        return
    
    #if path.find("case3") < 0:
    #    return

    testType = TestType.TestNormal
    for filename in os.listdir(path):
        print("filename is ",filename)
        if filename == "prepare.sh":
            isPrepareFileExist = True
        if filename == "makefile":
            isMakefileExist = True
        if filename == "client.go":
            testType = TestType.TestRunGoClient
        if filename == "server.go":
            testType = TestType.TestRunGoServer
        if filename == "client.py":
            testType = TestType.TestRunPythonClient
        if filename == "server.py":
            testType = TestType.TestRunPythonServer

    print("path trace2 type is ",testType)
    if isPrepareFileExist:
        os.popen("sh prepare.sh").read()
    if isMakefileExist:
        os.popen("cd " + path + "&& rm mytest").read()
        makeret = os.popen("cd " + path + "&& make 2>&1").read()
        try:
            port_file = open('/tmp/obotcha_test_suit_port.txt', 'r')
            list1 = port_file.readlines()
            if port_file:
                port_file.close()
        except IOError:
            os.popen("rm /tmp/obotcha_test_suit_port.txt").read()
        except UnicodeDecodeError:
            os.popen("rm /tmp/obotcha_test_suit_port.txt").read()

        go_build_success = True
        env_path =  os.path.abspath('.') + "/" + "../3rdparty/go"
        if testType == TestType.TestRunGoServer :
            gobuild = os.popen("export GOPATH=" 
                            + env_path + " && "
                            + "cd " + path + " && go build server.go ").read()
            makeret = makeret + "\r\n" + gobuild
        if testType == TestType.TestRunGoClient :
            print("export is export GOPATH=",env_path)
            gobuild = os.popen("export GOPATH=" 
                            + env_path + " && "
                            + "cd " + path + " && go build client.go ").read()
            makeret = makeret + "\r\n" + gobuild

        buildpath = path.split("/")
        buildfile = BUILD_REPORT_DIR + "/"
        executefile = EXECUTE_REPORT_DIR + "/"
        for ll in buildpath:
            print("build path ll is ",ll)
            if ll != "." and ll != ".." and len(ll) > 0 :
                buildfile += ll
                buildfile += "_"
                executefile += ll
                executefile += "_"

        #if makeret.find("Error") > 0:
        if os.path.exists(path+"/mytest"):
            buildfile += "Success.log"
        else:
            buildfile += "Fail.log"

        print("build result is ",buildfile)
        #write BuildReport
        file = open(buildfile,"w")
        file.write(makeret)
        file.close()

        #test
        global execThreadLog

        if os.path.exists(path+"/mytest"):
            printTime = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S.%f')[:-3]
            print("[" + printTime + "]" + " Start " + path)
            prepare(path)
            #check whether it is a cs test
            #executeret = ""

            if testType == TestType.TestNormal:
                execThreadLog = os.popen("cd " + path + " && ./mytest").read()
            if testType == TestType.TestRunPythonClient:
                print("start python client test")
                cmd = "cd " + path + " && ./mytest"
                t1 = threading.Thread(target= execByThread, args=(cmd,True), name='Thread-A')
                t1.start()
                
                #wait whether tmp file is ok
                time.sleep(0.1)
                waitTestData(path)
                time.sleep(0.2)

                pythonCmd = "cd " + path + " && python3 client.py"
                t2 = threading.Thread(target= execByThread, args=(pythonCmd,False), name='Thread-A')
                t2.start()
                t1.join();
                t2.join();
            if testType == TestType.TestRunPythonServer:
                pythonCmd = "cd " + path + " && python3 server.py"
                t2 = threading.Thread(target= execByThread, args=(pythonCmd,False), name='Thread-A')
                t2.start()
                time.sleep(0.5)

                cmd = "cd " + path + " && ./mytest"
                t1 = threading.Thread(target= execByThread, args=(cmd,True), name='Thread-A')
                t1.start()
                t1.join();
                t2.join();
            if testType == TestType.TestRunGoClient:
                cmd = "cd " + path + " && ./mytest"
                print("cmd is ",cmd)
                t1 = threading.Thread(target= execByThread, args=(cmd,True), name='Thread-A')
                t1.start()

                time.sleep(0.1)
                waitTestData(path)
                time.sleep(0.2)
                
                print("start go cmd")
                gocmd = "cd " + path + " && ./client"
                t2 = threading.Thread(target= execByThread, args=(gocmd,False), name='Thread-A')
                t2.start()
                t1.join();
                t2.join();
            if testType == TestType.TestRunGoServer:
                gocmd = "cd " + path + " && ./server"
                t1 = threading.Thread(target= execByThread, args=(gocmd,False), name='Thread-A')
                t1.start()
                time.sleep(0.3)
                cmd = "cd " + path + " && ./mytest"
                t2 = threading.Thread(target= execByThread, args=(cmd,True), name='Thread-A')
                t2.start()
                t1.join();
                t2.join();

            #executeret = execThreadLog
            #execThreadLog = ""
            print(execThreadLog)
            printTime = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S.%f')[:-3]
            print("[" + printTime + "]" + " Finish " + path)

            if execThreadLog.find("[FAIL]") > 0 or execThreadLog.find("[OK]") <= 0:
                executefile += "FAIL.log"
            else:
                executefile += "Success.log"
            
            file = open(executefile,"w")
            file.write(execThreadLog)
            file.close()
            execThreadLog = ""
        else:
            executefile = executefile + "Nottest.log"
            file = open(executefile,"w")
            file.write("not test");
            file.close()

def main():
    #remove report
    os.system("rm -rf " + REPORT_DIR)
    currentPath = os.path.abspath('.')
    #print("mkdir is ",currentPath + '/' + BUILD_REPORT_DIR)
    os.makedirs(currentPath + '/' + BUILD_REPORT_DIR)
    os.makedirs(currentPath + '/' + EXECUTE_REPORT_DIR)
    print("currentPath is ",currentPath)
    for path in testPath:
        for filename in os.listdir(path):
            testFolder = path + "/" + filename + "/";
            scanTest(testFolder)


if __name__ == '__main__':
    main()