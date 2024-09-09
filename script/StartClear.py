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
        folder_path = path + "/" + filename
        folder = Path(folder_path)
        if folder.is_dir():
            if filename == "tmp":
                if folder.is_dir():
                    os.popen("rm -rf " + folder_path).read()
            else:
                scan_items(folder_path)
        else:
            if filename == "mytest" or filename == "core":
                os.popen("rm " + folder_path).read()

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
    print("start clear...")
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
    print("finish clear...")

if __name__ == '__main__':
    main()