To build (-m32 -- build x86 32bit code, -fno-pic -- do not 
generate position independent code): 

  gcc -m32 -fno-pic hello-int.c

To look at secitons and disassembly (-s sections, -d disas):
  
  objdump -sd a.out

To compile a simple object file that fits on one screen 
(-c -- compile but don't link)
  
  gcc -m32 -c hello-int.c -o test

Another useful tool is readelf, to list all sections: 

  readelf -a test


Examples: 

------------------------------------------------------------------
- Simple program example from the lecture that computes 5 + 6
------------------------------------------------------------------

  gcc -m32 -fno-pic hello-int.c
  objdump -sd a.out

000004ed <main>:
 4ed:   55                      push   ebp
 4ee:   89 e5                   mov    ebp,esp
 4f0:   83 ec 10                sub    esp,0x10
 4f3:   c7 45 f8 05 00 00 00    mov    DWORD PTR [ebp-0x8],0x5
 4fa:   c7 45 fc 06 00 00 00    mov    DWORD PTR [ebp-0x4],0x6
 501:   8b 55 f8                mov    edx,DWORD PTR [ebp-0x8]
 504:   8b 45 fc                mov    eax,DWORD PTR [ebp-0x4]
 507:   01 d0                   add    eax,edx
 509:   c9                      leave  
 50a:   c3                      ret    
 50b:   66 90                   xchg   ax,ax
 50d:   66 90                   xchg   ax,ax
 50f:   90                      nop

------------------------------------------------------------------
- Global variables allocated in the data section
------------------------------------------------------------------

  gcc -m32 -fno-pic hello-static.c
  objdump -sd a.out

Contents of section .rodata:
 0688 03000000 01000200 25732025 730a00    ........%s %s..  // "%s %s\n" -- is allocated in the R/O data section

Contents of section .data:
 2000 00000000 04200000 48656c6c 6f000000  ..... ..Hello... // "Hello"  
 2010 776f726c 642100                      world!.          //  and "world!" are in  the data section

0000053d <main>:
 53d:   8d 4c 24 04             lea    ecx,[esp+0x4]
 541:   83 e4 f0                and    esp,0xfffffff0
 544:   ff 71 fc                push   DWORD PTR [ecx-0x4]
 547:   55                      push   ebp
 548:   89 e5                   mov    ebp,esp
 54a:   51                      push   ecx
 54b:   83 ec 04                sub    esp,0x4
 54e:   83 ec 04                sub    esp,0x4
 551:   68 10 20 00 00          push   0x2010           // Push address of the "world" on the stack as argument
 556:   68 08 20 00 00          push   0x2008           // Push address of "Hello " on the stack as argument
 55b:   68 00 06 00 00          push   0x600            // Push "%s %s\n" format string on the stack as argument
 560:   e8 fc ff ff ff          call   561 <main+0x24>  // Call printf

------------------------------------------------------------------
- Local variables example: now "world" is a local variable allocated on the stack 
------------------------------------------------------------------

  gcc -m32 -fno-pic hello-static.c
  objdump -sd a.out

Contents of section .data:
 2000 00000000 04200000 48656c6c 6f00      ..... ..Hello.  

0000059d <main>:
 59d:   8d 4c 24 04             lea    ecx,[esp+0x4]
 5a1:   83 e4 f0                and    esp,0xfffffff0
 5a4:   ff 71 fc                push   DWORD PTR [ecx-0x4]
 5a7:   55                      push   ebp
 5a8:   89 e5                   mov    ebp,esp
 5aa:   51                      push   ecx
 5ab:   83 ec 24                sub    esp,0x24
 5ae:   89 c8                   mov    eax,ecx
 5b0:   8b 40 04                mov    eax,DWORD PTR [eax+0x4]
 5b3:   89 45 e4                mov    DWORD PTR [ebp-0x1c],eax
 5b6:   65 a1 14 00 00 00       mov    eax,gs:0x14
 5bc:   89 45 f4                mov    DWORD PTR [ebp-0xc],eax
 5bf:   31 c0                   xor    eax,eax
 5c1:   c7 45 ed 77 6f 72 6c    mov    DWORD PTR [ebp-0x13],0x6c726f77    // 0x77 - "w", 0x6f - "o", 0x72 - "r", 0x6c - "l"
 5c8:   66 c7 45 f1 64 21       mov    WORD PTR [ebp-0xf],0x2164          // 0x64 - "d", 0x21 - "!" 
 5ce:   c6 45 f3 00             mov    BYTE PTR [ebp-0xd],0x0             // 0x0 - "\n"
 5d2:   83 ec 04                sub    esp,0x4
 5d5:   8d 45 ed                lea    eax,[ebp-0x13]
 5d8:   50                      push   eax                                // Push address of "world!\n" as an argument on the stack
 5d9:   68 08 20 00 00          push   0x2008                             // Push "Hello" on the stack
 5de:   68 90 06 00 00          push   0x690                              // Push "%s %s\n" format string on the stack as argument
 5e3:   e8 fc ff ff ff          call   5e4 <main+0x47>                    // Call printf
 5e8:   83 c4 10                add    esp,0x10
 5eb:   b8 00 00 00 00          mov    eax,0x0
 5f0:   8b 55 f4                mov    edx,DWORD PTR [ebp-0xc]
 5f3:   65 33 15 14 00 00 00    xor    edx,DWORD PTR gs:0x14
 5fa:   74 05                   je     601 <main+0x64>
 5fc:   e8 fc ff ff ff          call   5fd <main+0x60>
 601:   8b 4d fc                mov    ecx,DWORD PTR [ebp-0x4]
 604:   c9                      leave  
 605:   8d 61 fc                lea    esp,[ecx-0x4]
 608:   c3                      ret    
 609:   66 90                   xchg   ax,ax
 60b:   66 90                   xchg   ax,ax
 60d:   66 90                   xchg   ax,ax
 60f:   90                      nop



------------------------------------------------------------------------
-- Relocation example
------------------------------------------------------------------------

gcc -m32 -fno-pic -c hello-reloc.c
objdump -M intel -dr hello-reloc.o

Disassembly of section .text:

00000000 <add>:
   0:   55                      push   ebp
   1:   89 e5                   mov    ebp,esp
   3:   8b 55 08                mov    edx,DWORD PTR [ebp+0x8]
   6:   8b 45 0c                mov    eax,DWORD PTR [ebp+0xc]
   9:   01 d0                   add    eax,edx
   b:   5d                      pop    ebp
   c:   c3                      ret    

0000000d <main>:
   d:   8d 4c 24 04             lea    ecx,[esp+0x4]
  11:   83 e4 f0                and    esp,0xfffffff0
  14:   ff 71 fc                push   DWORD PTR [ecx-0x4]
  17:   55                      push   ebp
  18:   89 e5                   mov    ebp,esp
  1a:   51                      push   ecx
  1b:   83 ec 14                sub    esp,0x14
  1e:   c7 45 f0 04 00 00 00    mov    DWORD PTR [ebp-0x10],0x4            // b is local (allocated on the stack)
  25:   a1 00 00 00 00          mov    eax,ds:0x0                          // a is in the data section will have to be relocated
                        26: R_386_32    a
  2a:   ff 75 f0                push   DWORD PTR [ebp-0x10]
  2d:   50                      push   eax
  2e:   e8 cd ff ff ff          call   0 <add>                             // add() is a static function, we can call it with 0xcd (-51 bytes offset, i.e. 0x0 - 0x33
  33:   83 c4 08                add    esp,0x8
  36:   89 45 f4                mov    DWORD PTR [ebp-0xc],eax
  39:   83 ec 08                sub    esp,0x8
  3c:   ff 75 f4                push   DWORD PTR [ebp-0xc]
  3f:   68 00 00 00 00          push   0x0
                        40: R_386_32    .rodata                            // "Result: %u\n" is in the .rodata section will have to be relocated
  44:   e8 fc ff ff ff          call   45 <main+0x38>                      // "printf" is undefined, will have to be found
                        45: R_386_PC32  printf
  49:   83 c4 10                add    esp,0x10
  4c:   b8 00 00 00 00          mov    eax,0x0
  51:   8b 4d fc                mov    ecx,DWORD PTR [ebp-0x4]
  54:   c9                      leave  
  55:   8d 61 fc                lea    esp,[ecx-0x4]
  58:   c3                      ret    


---------------------------------------------------------------------------
-- Simple ELF executable
---------------------------------------------------------------------------

gcc -c -fno-pic -static -fno-builtin -ggdb -m32 -fno-omit-frame-pointer hello-elf.c
ld -m elf_i386 -N -e main -Ttext 0 -o a.out hello-elf.o

readelf -a a.out

---------------------------------------------------------------------------
-- Link static executable 
---------------------------------------------------------------------------

gcc -m32 -fno-pic --static hello-reloc.c

ldd a.out

objdump -drR a.out

---------------------------------------------------------------------------
-- Link dynamic executable 
---------------------------------------------------------------------------

gcc -m32 -fno-pic hello-reloc.c

ldd a.out 
	linux-gate.so.1 (0xf7fb0000)
	libc.so.6 => /lib/i386-linux-gnu/libc.so.6 (0xf7dae000)
	/lib/ld-linux.so.2 (0xf7fb1000)

objdump -dR a.out

---------------------------------------------------------------------------
-- Link static executable 
---------------------------------------------------------------------------

gcc -m32 -fno-pic --static hello-reloc.c

---------------------------------------------------------------------------
-- Link static and dynamic PIC executables
---------------------------------------------------------------------------

gcc -m32 --static hello-reloc.c
gcc -m32 hello-reloc.c

