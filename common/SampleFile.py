import os
import random
def create_sample_file(path,size):
    with open(path + "/tmp/testdata", 'wb') as f:
        f.write(os.urandom(size))
