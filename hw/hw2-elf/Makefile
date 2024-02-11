#This is how you write comments
# Use gcc as a compiler
CC=gcc
# CFLAGS will be the options we'll pass to the compiler
CFLAGS=-Wall -m32 -g -fno-pic

all: main elf

main: main.c
	$(CC) $(CFLAGS) main.c -o main

elf: elf.o 
	ld -m elf_i386 -N -e sum -Ttext 0 -static --emit-relocs -o elf elf.o 

elf.o: elf.c
	$(CC) -c -fno-pic -static -fno-builtin -m32 -fno-omit-frame-pointer -fno-asynchronous-unwind-tables -o elf.o elf.c 

clean:
	rm *.o main elf

