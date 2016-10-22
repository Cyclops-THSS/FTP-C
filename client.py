import socket
import sys

size = 8192
sock = None

try:
    sock = socket.create_connection(('localhost', 21))
    while True:
        res = sock.recv(size).decode()
        if res is '':
            raise Exception('Server does not respond')
        print(res)
        msg = sys.stdin.readline()
        sock.sendall(msg.replace('\n', '\r\n').encode())
except KeyboardInterrupt:
    print("bye")
except Exception as e:
    print(e)
finally:
    if sock:
        sock.close()
