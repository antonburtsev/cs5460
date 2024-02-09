#This is how you write comments
# Use gcc as a compiler
CC=gcc
# CFLAGS will be the options we'll pass to the compiler
CFLAGS=-Wall -m32 -g -fno-pic

all: main elf

main: main.c
	$(CC) $(CFLAGS) main.c -o main

elf: elf-data.o 
	ld -m elf_i386 -N -e main -Ttext 0 -static --emit-relocs -o elf elf-data.o 

elf-data.o: elf-data.c
	$(CC) -c -fno-pic -static -fno-builtin -ggdb -m32 -fno-omit-frame-pointer -o elf-data.o elf-data.c 

clean:
	rm *.o main elf

