# Use gcc as a compiler
CC=gcc
# CFLAGS will be the options we'll pass to the compiler
CFLAGS=-Wall -m32 -g -fno-pic

all: main elf

main: main.c
	$(CC) $(CFLAGS) main.c -o main

elf: elf.c
	$(CC) -m32 -nostdlib -fno-asynchronous-unwind-tables -fno-plt -fno-pic -static-pie -Wl,-N -e quadruple -o $@ $<

clean:
	rm -f *.o main elf
