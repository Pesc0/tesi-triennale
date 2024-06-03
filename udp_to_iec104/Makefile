CFLAGS = -Wall
INCLUDES = -I../iec104/include
LDFLAGS = ../iec104/libx86_x64-iec104.a -lpthread -lrt


all:
	gcc udp_to_iec104.c $(CFLAGS) $(INCLUDES) $(LDFLAGS) -o udp_to_iec104
