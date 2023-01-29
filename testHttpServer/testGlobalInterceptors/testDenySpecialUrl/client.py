import socket
import threading
import time
import http.client

import sys
sys.path.append(r'../../../common')
from NetPort import getEnvPort
from NetPort import setEnvPort

count = 0

port = getEnvPort()
port = 3006
url = "127.0.0.1:" + str(port)

client = http.client.HTTPConnection(url)
client.request("GET","/simpleget")
r1 = client.getresponse()
responseBody = r1.read().decode("utf-8");
status = r1.getcode();

if status == 204:
    print("success1")
    client = http.client.HTTPConnection(url)
    client.request("Post","/okget",body=responseBody)
    r1 = client.getresponse()
    status = r1.getcode();
    print("status is " + str(status))

    if status == 200:
        print("success2")
        client = http.client.HTTPConnection(url)
        client.request("GET","/complete")
        r1 = client.getresponse()
        responseBody = r1.read().decode("utf-8");


client.close();

print("finish!!!")