from pathlib import Path
import os
def scan_items(path):
    for filename in os.listdir(path):
        folder = Path(path + "/" + filename)
        if folder.is_dir():
            scan_items(path+"/" + filename)
    os.popen("cd " + path + "&& rm -rf mytest core client server tmp").read()
def main():
    print("Clear Start")
    scan_items("../testIo")
    scan_items("../testLang")
    scan_items("../testNet")
    scan_items("../testProcess")
    scan_items("../testSecurity")
    scan_items("../testSql")
    print("Clear Complete")

if __name__ == '__main__':
    main()