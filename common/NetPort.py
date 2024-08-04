def getEnvPort():
    try:
        f = open('/tmp/obotcha_test_suit_port.txt', 'r')
        list1 = f.readlines()
        if f:
            f.close()
        
        if len(list1) <= 0:
            return 4000

        return int(list1[0])
    except IOError:
        return 4000 
    
def setEnvPort(port):
    if port > 60000:
        port = 4000
    fo = open("/tmp/obotcha_test_suit_port.txt", "w")
    fo.write(str(port))
    fo.close()

def resetEnvPort():
    setEnvPort(4000)
