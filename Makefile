CC = gcc
CFLAGS = -Wall -std=c11

all: virtual-machine

virtual-machine: virtual-machine.o
	$(CC) $(CFLAGS) -o virtual-machine virtual-machine.o

virtual-machine.o: virtual-machine.c
	$(CC) $(CFLAGS) -c virtual-machine.c -o virtual-machine.o

clean:
	rm -rf virtual-machine virtual-machine.o
