import socket
import struct
import time

ADDR = ("127.0.0.1", 5005)

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

with open("random_data.txt", "r") as f:
    for row in f.readlines():
        s.sendto(struct.pack("f", float(row)), ADDR)
        time.sleep(0.01)
