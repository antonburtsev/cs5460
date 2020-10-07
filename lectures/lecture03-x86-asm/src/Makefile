elf: main.o
	ld -m elf_i386 -N -e main -Ttext 0 -o main main.o 

main.o: main.c
	$(CC) -c -fno-pic -static -fno-builtin -ggdb -m32 main.c
