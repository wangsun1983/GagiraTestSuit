import socket
import threading
import time
import http
import requests
from requests_toolbelt.multipart.encoder import MultipartEncoder
import random
import os

import sys
sys.path.append(r'../../../common')
from NetPort import getEnvPort
from NetPort import setEnvPort

threads = []
url = "http://127.0.0.1:" + str(getEnvPort()) + "/simplepost/1111"

def multiUpload(start: int) -> None:
    count = 0
    while count < 32:
        headers = {
            'User-Agent': 'Mozilla/5.0 (Windows NT 6.1; Win64; x64; rv:50.0) Gecko/20100101 Firefox/50.0',
        }


        multipart_encoder = MultipartEncoder(
            fields = {
                'file': (os.path.basename("data"), open("./tmp/testdata", 'rb'), 'multipart/form-data'),
                'save_name': 'data',
                'key1':'value1'
            },
            boundary = '-----------------------------' + str(random.randint(1e28, 1e29 - 1))
        )

        headers['Content-Type'] = multipart_encoder.content_type

        #print("send start")
        requests.post(url, data=multipart_encoder, headers=headers)
        #responseStr.read();
        count = count + 1
        #print("send complete")

for i in range(0, 8):
    t = threading.Thread(target=multiUpload, args=(i, ))
    t.start()
    threads.append(t)

for t in threads:
    t.join()

print("get response")
    #print(response.text)