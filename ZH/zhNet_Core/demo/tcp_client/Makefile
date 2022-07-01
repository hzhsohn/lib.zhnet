# Project: client

CC = /bin/gcc
RES  = 
OBJ  = main.o ../lib/platform.o ../lib/packet.o ../lib/socket.o ../lib/session.o ../lib/packet_heap.o ../lib/netlist.o $(RES)
LINKOBJ  = main.o ../lib/platform.o ../lib/packet.o ../lib/socket.o ../lib/session.o ../lib/packet_heap.o ../lib/netlist.o $(RES)
LIBS =  
INCS = -I ../lib
CXXINCS = 
BIN  = ./client.exe
CXXFLAGS = $(CXXINCS)  
CFLAGS = $(INCS)  
RM = rm -f

.PHONY: all all-before all-after clean clean-custom

all: all-before ./client.exe all-after


clean: clean-custom
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CC) $(LINKOBJ) -o "./client.exe" -pthread$(LIBS)

main.o: main.c
	$(CC) -c main.c -o main.o $(CFLAGS)

../lib/platform.o: ../lib/platform.c
	$(CC) -c ../lib/platform.c -o ../lib/platform.o $(CFLAGS)

../lib/packet.o: ../lib/packet.c
	$(CC) -c ../lib/packet.c -o ../lib/packet.o $(CFLAGS)

../lib/socket.o: ../lib/socket.c
	$(CC) -c ../lib/socket.c -o ../lib/socket.o $(CFLAGS)

../lib/session.o: ../lib/session.c 
	$(CC) -c ../lib/session.c -o ../lib/session.o $(CFLAGS)

../lib/packet_heap.o: ../lib/packet_heap.c
	$(CC) -c ../lib/packet_heap.c -o ../lib/packet_heap.o $(CFLAGS)

../lib/netlist.o: ../lib/netlist.c
	$(CC) -c ../lib/netlist.c -o ../lib/netlist.o $(CFLAGS)

