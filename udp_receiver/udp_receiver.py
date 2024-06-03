import socket
import struct

BUF_SIZE = 65535
ADDR = ("127.0.0.1", 5005)

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.bind(ADDR)

while True:
    data = s.recvfrom(BUF_SIZE)[0]
    decoded = struct.unpack("f", data)[0]
    print(f"received {decoded}")
