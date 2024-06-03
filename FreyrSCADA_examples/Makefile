
CFLAGS = -Wall -O2
INCLUDES = -I../iec104/include
LDFLAGS = ../iec104/libx86_x64-iec104.a -lpthread -lrt


all:
	gcc simpleIEC104server.c terminalinput.c $(CFLAGS) $(INCLUDES) $(LDFLAGS) -o iec104servertest
	gcc simpleIEC104client.c terminalinput.c $(CFLAGS) $(INCLUDES) $(LDFLAGS) -o iec104clienttest
