import websocket
import threading

import sys
sys.path.append(r'../../../common')
from NetPort import getEnvPort
from NetPort import setEnvPort

url = "ws://127.0.0.1:" + str(getEnvPort()) + "/simpleget"
ws = websocket.WebSocket()
ws.connect(url)
msg = "Hello, World"
ws.send(msg)
ws.close()

url = "ws://127.0.0.1:" + str(getEnvPort()) + "/complete"
ws = websocket.WebSocket()
ws.connect(url)
msg = "He"
ws.send(msg)
ws.close()