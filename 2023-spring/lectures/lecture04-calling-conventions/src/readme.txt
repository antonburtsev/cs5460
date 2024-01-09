To build a simple object file that fits on one screen
(-c -- compile but don't link): 

  gcc -m32 -fno-pic -g -c main.c
  gcc -m32 -O -fno-omit-frame-pointer -c foo.c

To look at disassembly -d disas (Intel syntax):

  objdump -M intel -d main.o

