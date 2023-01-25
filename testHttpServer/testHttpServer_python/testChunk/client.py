import socket
import threading
import time
import http.client
import requests
import os


import sys
sys.path.append(r'../../../common')
from NetPort import getEnvPort
from NetPort import setEnvPort

#while 1:
count = 0
url = "http://127.0.0.1:" + str(getEnvPort()) + "/simpleget"

while count < 32:
    r = requests.get(url)
    filename = "./tmp/"+str(count)
    with open(os.path.join(os.path.dirname(os.path.abspath("__file__")),filename),"wb") as f:
        f.write(r.content)
    count = count + 1
    r.close()

client = http.client.HTTPConnection("127.0.0.1:" + str(getEnvPort()))
client.request("GET","/completeget")
r1 = client.getresponse()

client.close();

print("finished")