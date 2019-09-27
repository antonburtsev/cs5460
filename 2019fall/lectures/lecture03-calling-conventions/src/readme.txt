To build a simple object file that fits on one screen
(-c -- compile but don't link): 

  gcc -m32 -c main.c test

To look at disassembly -d disas (GCC syntax):
  
  objdump -d test

Same as above but Intel syntax:

  objdump -M intel -d test 
