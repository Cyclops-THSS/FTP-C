import socket

size = 8192
sock = None

try:
    for count in range(0, 51):
        msg = str(count)
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        sock.sendto(msg, ('localhost', 9876))
        print sock.recv(size)
    sock.close()
except KeyboardInterrupt:
    if sock:
        sock.close()
    print "bye"
except:
    print "cannot reach the server"
