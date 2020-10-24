
nasm_rc: nasm_rc.o
	ld -m elf_i386 -o nasm_rc nasm_rc.o

nasm_rc_0: nasm_rc.o
	ld -m elf_i386 -Ttext 0 -o nasm_rc nasm_rc.o

nasm_rc.o: nasm_rc.asm
	nasm -f elf32 nasm_rc.asm -o nasm_rc.o

objdump_nasm:
	objdump -M intel -d nasm_rc32 

c_rc: c_rc.o
	gcc -Wl,-verbose -m32 -o c_rc -static c_rc.o

c_rc_ld:c_rc.o
	ld -m elf_i386 -o c_rc c_rc.o 

c_rc.o: c_rc.c 
	gcc -c -m32 -fno-pic -fno-builtin -fno-omit-frame-pointer c_rc.c 

objdump_c:
	objdump -M intel -d c_rc 

clean:
	rm nasm_rc.o nasm_rc c_rc.o c_rc


