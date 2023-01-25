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
url = "127.0.0.1:" + str(port)

threads = []

def multiDownload(start: int) -> None:
    count = 0;
    while count < 32:
        client = http.client.HTTPConnection(url)
        client.request("GET","/simpleget/" + str(count))
        r1 = client.getresponse()
        count = count + 1

for i in range(0, 32):
    t = threading.Thread(target=multiDownload, args=(i, ))
    t.start()
    threads.append(t)

for t in threads:
    t.join()

client = http.client.HTTPConnection(url)
client.request("GET","/complete")
r1 = client.getresponse()

client.close();

print("finish!!!")