import os
import datetime
import threading
import time
from enum import Enum
from pathlib import Path

import subprocess

#prepare
#pip3 install websocket-client
REPORT_DIR = '../Report'
BUILD_REPORT_DIR ='../Report/BuildReport'
EXECUTE_REPORT_DIR ='../Report/ExecuteReport'


class ItemType(Enum):
    TestNormal             = 1
    TestRunGoClient        = 2
    TestRunGoServer        = 3
    TestRunPythonClient    = 4
    TestRunPythonServer    = 5

build_items = []

def scan_items(path):
    for filename in os.listdir(path):
        if filename == "makefile":
            build_items.append(path)
            break
        
        folder_path = path + "/" + filename
        folder = Path(folder_path)
        if folder.is_dir():
            if folder_path.endswith("skip") > 0:
                return
            scan_items(folder_path)

def prepare_port_record_file():
    try:
        port_file = open('/tmp/obotcha_test_suit_port.txt', 'r')
        list1 = port_file.readlines()
        if port_file:
            port_file.close()
    except IOError:
        os.popen("rm /tmp/obotcha_test_suit_port.txt").read()
    except UnicodeDecodeError:
        os.popen("rm /tmp/obotcha_test_suit_port.txt").read()

#def prepare_tmp_folder(path):
#    os.popen("rm -rf " + path + "/tmp").read()
#    while os.path.exists(path + "/tmp"):
#        time.sleep(0.1)
#        print("path " + path + "tmp already exist,wait....")
#    os.popen("mkdir " + path + "/tmp").read()

def write_result(path,content):
    error = False
    if path != "":
        if content.find("[FAIL]") > 0 or content.find("[OK]") <= 0:
            path += "FAIL.log"
            error = True
        else:
            path += "Success.log"

        execute_file = open(path,"w")
        execute_file.write(content)
        execute_file.close()


def prepare_item_type(path):
    item_type = ItemType.TestNormal
    is_need_prepare = False
    is_need_build = False


    #clear tmp data/core/mytest
    for filename in os.listdir(path):
        if filename == "prepare.sh":
            is_need_prepare = True
        if filename == "makefile":
            is_need_build = True
        if filename == "client.go":
            item_type = ItemType.TestRunGoClient
        if filename == "server.go":
            item_type = ItemType.TestRunGoServer
        if filename == "client.py":
            item_type = ItemType.TestRunPythonClient
        if filename == "server.py":
            item_type = ItemType.TestRunPythonServer
    
    #do prepare 
    if is_need_prepare:
        build_result = os.popen("cd " + path + "&& chmod 777 prepare.sh && ./prepare.sh").read()
    
    if is_need_build:
        build_result = os.popen("cd " + path + "&& make 2>&1").read()
    
    env_path =  os.path.abspath('.') + "/" + "../3rdparty/go"
    if item_type == ItemType.TestRunGoServer:
        go_build_result = os.popen("export GOPATH=" 
                            + env_path + " && "
                            + "cd " + path + " && go build server.go ").read()
        build_result += "\\r\\n" + go_build_result
    elif item_type == ItemType.TestRunGoClient:
        go_build_result = os.popen("export GOPATH=" 
                        + env_path + " && "
                        + "cd " + path + " && go build client.go ").read()
        build_result = "\\r\\n" + go_build_result


    build_dir_items = path.split("/")
    build_file_path = BUILD_REPORT_DIR + "/"
    execute_result_path = EXECUTE_REPORT_DIR + "/"
    for ll in build_dir_items:
        if ll != "." and ll != ".." and len(ll) > 0 :
            build_file_path += ll
            build_file_path += "_"
            execute_result_path += ll
            execute_result_path += "_"
    
    if os.path.exists(path + "/mytest"):
        build_file_path += "Success.log"
    else:
        build_file_path += "Fail.log"

    #write BuildReport
    build_file = open(build_file_path,"w")
    build_file.write(build_result)
    build_file.close()

    if os.path.exists(path+"/mytest"):
        prepare_port_record_file()
        return (execute_result_path,item_type)
    else:
        execute_result_path = execute_result_path + "Nottest.log"
        execute_file = open(execute_result_path,"w")
        execute_file.write("not test")
        execute_file.close()
        return ("",item_type)

def wait_test_data(path):
    filesize = 0
    if os.path.exists(path + "/tmp/testdata"):
        while True:
            filesize = os.path.getsize(path + "/tmp/testdata")
            time.sleep(0.1)
            if filesize == os.path.getsize(path + "/tmp/testdata"):
                return

def execute_with_thread(execute_result_path,command):
    execute_result = os.popen(command).read()
    
    if execute_result_path != "":    
        if execute_result.find("[FAIL]") > 0 or execute_result.find("[OK]") <= 0 or execute_result.find("AddressSanitizer") > 0:
            execute_result_path += "FAIL.log"
        else:
            execute_result_path += "Success.log"

        execute_file = open(execute_result_path,"w")
        execute_file.write(execute_result)
        execute_file.close()

def clear_old_files(path):
    os.popen("rm -rf " + path + "/tmp").read()
    while os.path.exists(path + "/tmp"):
        time.sleep(0.1)
        print("path " + path + "tmp already exist,wait....")
    os.popen("cd " + path + "&& rm -rf mytest core client server").read()

def create_mytest_command(path):
    return "ulimit -c unlimited && export ASAN_OPTIONS=disable_coredump=0:unmap_shadow_on_exit=1:abort_on_error=1:core_directory=./:log_path=stdout && cd " + path + " && ./mytest"

def start_test():
    for item in build_items:
        do_test(item)

def do_test(item_path):
    print("[",item_path,"] Start")
    clear_old_files(item_path)
    os.popen("mkdir " + item_path + "/tmp").read()
    (execute_result_path,item_type) = prepare_item_type(item_path)
    if execute_result_path == "":
        print("[",item_path,"] No Need Test")
        return
    
    execute_result = ""
    if item_type == ItemType.TestNormal:
        execute_result = os.popen(create_mytest_command(item_path)).read()
        if execute_result.find("[FAIL]") > 0 or execute_result.find("[OK]") <=0 or execute_result.find("AddressSanitizer") > 0:
            execute_result_path += "FAIL.log"
        else:
            execute_result_path += "Success.log"

        execute_file = open(execute_result_path,"w")
        execute_file.write(execute_result)
        execute_file.close()
    elif item_type == ItemType.TestRunPythonClient:
        cmd = create_mytest_command(item_path)
        server_thread = threading.Thread(target= execute_with_thread, 
                                            args=(execute_result_path,cmd), 
                                            name='Thread-A')
        server_thread.start()

        time.sleep(0.5)
        wait_test_data(item_path)
        time.sleep(0.5)

        client_cmd = "cd " + item_path + " && python3 client.py"
        client_thread = threading.Thread(target= execute_with_thread, 
                                            args=("",client_cmd), 
                                            name='Thread-B')
        client_thread.start()
        server_thread.join()
        client_thread.join()
    elif item_type == ItemType.TestRunPythonServer:
        python_cmd = "cd " + item_path + " && python3 server.py"
        server_thread = threading.Thread(target= execute_with_thread, 
                                            args=("",python_cmd), 
                                            name='Thread-A')
        server_thread.start()
        time.sleep(0.2)
        wait_test_data(item_path)
        time.sleep(0.2)

        client_cmd = create_mytest_command(item_path)
        client_thread = threading.Thread(target= execute_with_thread, 
                                            args=(execute_result_path,client_cmd), 
                                            name='Thread-B')
        client_thread.start()
        
        client_thread.join()
        server_thread.join()
    elif item_type == ItemType.TestRunGoClient:
        cmd = create_mytest_command(item_path)
        server_thread = threading.Thread(target= execute_with_thread, 
                                            args=(execute_result_path,cmd), 
                                            name='Thread-A')
        server_thread.start()

        time.sleep(0.5)
        wait_test_data(item_path)
        time.sleep(0.5)
        
        client_cmd = "cd " + item_path + " && ./client"
        client_thread = threading.Thread(target= execute_with_thread, 
                                        args=("",client_cmd), 
                                        name='Thread-B')
        client_thread.start()
        client_thread.join()
        server_thread.join()
    elif item_type == ItemType.TestRunGoServer:
        gocmd = "cd " + item_path + " && ./server"
        server_thread = threading.Thread(target= execute_with_thread, 
                                            args=("",gocmd), 
                                            name='Thread-A')
        server_thread.start()
        time.sleep(0.3)
        cmd = create_mytest_command(item_path)
        client_thread = threading.Thread(target= execute_with_thread, 
                                            args=(execute_result_path,cmd), name='Thread-B')
        client_thread.start()

        client_thread.join()
        server_thread.join()

    print("[",item_path,"] End")

def dump_build_items():
    #for path in build_items:
    #    print(path)
    f = open("./build_items.txt","w")
    for path in build_items:
        f.write(path + "\\r\\n")
    f.close()

def prepare_report():
    os.system("rm -rf " + REPORT_DIR)
    currentPath = os.path.abspath('.')
    os.makedirs(currentPath + '/' + BUILD_REPORT_DIR)
    os.makedirs(currentPath + '/' + EXECUTE_REPORT_DIR)

def print_logo():
    print("")
    print("================================================================================================")
    print("  ________               .__  \r\n" +             
          "/  _____/_____     ____ |__|___________\r\n" +   
          "/   \\  ___\\__  \\   / ___\\|  \\_  __ \\__  \\\r\n" +  
          "\\    \\_\\  \\/ __ \\_/ /_/  >  ||  | \\// __ \\_\r\n" +
          " \\______  (____  /\\___  /|__||__|  (____  /\r\n" +
          "        \\/     \\//_____/                \\/ by sunli.wang")
    print("")
    print("================================================================================================")
    print("")

def main():
    print_logo()
    prepare_report()
    #scan_items("/home/test/wangsun/mysource/Obotcha/ObotchaTestSuite/testIo/testAsyncOutputChannel/sendClose/")
    #scan_items("../testNet/testHttp/testHttpServer_python3/testChunckFileServer")
    #scan_items("/home/test/wangsun/mysource/Obotcha/ObotchaTestSuite/testNet/testWebsocket/testWebSocketClient/")    
    #scan_items("/home/test/wangsun/mysource/Obotcha/ObotchaTestSuite/testNet/testHttp2_skip")
    scan_items("../testArchive")
    #scan_items("../testBroadcast")
    #scan_items("../testFence")
    #scan_items("../testHttpRouterMap")
    #scan_items("../testHttpServer")
    #scan_items("../testQueue")
    #scan_items("../testSpace")
    #scan_items("../testTemplate")
    #scan_items("../testSql")
    dump_build_items()
    start_test()

if __name__ == '__main__':
    main()