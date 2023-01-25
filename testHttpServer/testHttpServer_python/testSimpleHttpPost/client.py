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
client.request("GET","/simpleget/100")
r1 = client.getresponse()
responseBody = r1.read().decode("utf-8");

client = http.client.HTTPConnection(url)
client.request("Post","/simplepost2/200",body=responseBody)
r1 = client.getresponse()

client.close();

print("finish!!!")