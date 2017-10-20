To build: 

  gcc -m32 hello-int.c

To look at secitons and disassembly (-s sections, -d disas):
  
  objdump -sd a.out

To compile a simple object file that fits on one screen
  
  gcc -m32 -c hello-undef.c -o test

Another useful tool is readelf, to list all sections: 

  readelf -a test



