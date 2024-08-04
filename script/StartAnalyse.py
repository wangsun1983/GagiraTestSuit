import os
import datetime
import threading
import time
from enum import Enum
from pathlib import Path

functions = []

def start_objdump(path):
    functions = []
    objdump_result = os.popen("objdump -x " + path).read()
    objdump_lines = objdump_result.splitlines()
    for line in objdump_lines:
        if line.find(".text") > 0:
            #convert function
            endIndex = line.rfind(" ")
            function_name = line[endIndex+1:]
            filter_name = os.popen("c++filt " + function_name).read()
            if filter_name.find("obotcha::_") == 0:
                functions.append(filter_name)
    
    #for name in functions:
    #    print(name)
    
    return functions

obotcha_functions = []
def start_items(path):
    for filename in os.listdir(path):
        if filename == "mytest":
            functions_2 = start_objdump(path + "/mytest")
            for function in functions_2:
                try:
                    obotcha_functions.remove(function)
                except:
                    continue
        else:    
            folder = Path(path + "/" + filename)
            if folder.is_dir():
                if path.find("skip") > 0:
                    return
                start_items(path+"/" + filename)
        

def main():
    obotcha_functions = start_objdump("../../out/lib/libobotcha.so")
    print("size is ",len(obotcha_functions))
    start_items("../testLang")
    
    f = open("./analyse_interface_items.txt","w")
    for func in obotcha_functions:
        f.write(func)
    f.close()

if __name__ == '__main__':
    main()