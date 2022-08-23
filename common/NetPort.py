def getEnvPort():
    try:
        f = open('/tmp/obotcha_test_suit_port.txt', 'r')
        list1 = f.readlines()
        if f:
            f.close()
        return int(list1[0])
    except IOError:
        return 2001 
    
def setEnvPort(port):
    if port > 60000:
        port = 2001
    fo = open("/tmp/obotcha_test_suit_port.txt", "w")
    fo.write(str(port))
    fo.close()

def resetEnvPort():
    setEnvPort(2001)
