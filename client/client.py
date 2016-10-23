import sys
from ftplib import FTP
ftp = FTP('localhost')
try:
    ftp.login()
    while True:
        line = sys.stdin.readline()
        print(bytes(str(ftp.sendcmd(line)).encode()))
except Exception as e:
    print(bytes(str(e).encode()))
finally:
    ftp.close()
