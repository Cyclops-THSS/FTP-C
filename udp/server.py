import socket

size = 8192
count = 0

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(('', 9876))

try:
    while True:
        data, address = sock.recvfrom(size)
        count += 1
        sock.sendto(str(count) + str(' ') + data.upper(), address)
except KeyboardInterrupt:
	print "bye"
finally:
    sock.close()
