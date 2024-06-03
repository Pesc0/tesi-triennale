CFLAGS = -Wall
INCLUDES = -I../iec104/include
LDFLAGS = ../iec104/libx86_x64-iec104.a -lpthread -lrt


all:
	gcc iec104_sender.c $(CFLAGS) $(INCLUDES) $(LDFLAGS) -o iec104_sender
