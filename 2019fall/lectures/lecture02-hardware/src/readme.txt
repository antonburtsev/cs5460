To build a simple object file that fits on one screen
(-c -- compile but don't link): 

  gcc -m32 -fno-pic -fomit-frame-pointer -c main.c

To look at disassembly -d disas (GCC syntax):
  
  objdump -dr main.o

Same as above but Intel syntax:

  objdump -M intel -dr main.o
