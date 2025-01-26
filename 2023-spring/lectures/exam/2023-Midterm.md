# Q2

Read: https://stackoverflow.com/questions/10505690/what-is-the-meaning-of-lea-0x0esi-esi


```
Summary: 
ecx is const char *s
edx is int n


ENTER
-------------------------------------------------------------
199 00000200 <strlen>:
200  200:   55                      push   ebp                         |
201  201:   89 e5                   mov    ebp,esp                     |   Save prev stack frame, ENTER instruction
--------------------------------------------------------------

n = 0; 
set ecx = s
set edx = n = 0
---------------------------------------------------------
202  203:   8b 4d 08                mov    ecx,DWORD PTR [ebp+0x8]     |  ecx is char* s  
203  206:   80 39 00                cmp    BYTE PTR [ecx],0x0          |  for loop exit condition,  s[0] == 0, then program will return n, which is 0. 
204  209:   74 15                   je     220 <strlen+0x20>           |  Exit for loop
205  20b:   31 d2                   xor    edx,edx                     |  edx has content of variable n. n = 0 

206  20d:   8d 76 00                lea    esi,[esi+0x0]               |  --> https://stackoverflow.com/questions/10505690/what-is-the-meaning-of-lea-0x0esi-esi
                                                                       |  --> fast NOP

----------------------------------------------------------------

for(n = 0; s[n]; n++) ;                                                                    
---------------------------------------------------------------------
207  210:   83 c2 01                add    edx,0x1                     |  n++ 
208  213:   80 3c 11 00             cmp    BYTE PTR [ecx+edx*1],0x0    |  s[n] == 0 
209  217:   89 d0                   mov    eax,edx                     |  prepare to return n
210  219:   75 f5                   jne    210 <strlen+0x10>           |  if s[n] != 0, then we keep accumulate n. 
---------------------------------------------------------------

return n; 
---------------------------------------------------------------
211  21b:   5d                      pop    ebp                         |  else we return n, restore stack frame
212  21c:   c3                      ret                                |

213  21d:   8d 76 00                lea    esi,[esi+0x0]               | NOP
----------------------------------------------------------

return 0;
----------------------------------------------------------
214  220:   31 c0                   xor    eax,eax                     | exit for loop, return n ( n is 0 ) and xor eax eax will make eax 0. 
215  222:   5d                      pop    ebp                         | restore ebp, LEAVE instruction
216  223:   c3                      ret                                | return 
-------------------------------------------------------
217  224:   8d b6 00 00 00 00       lea    esi,[esi+0x0]               | NOP
218  22a:   8d bf 00 00 00 00       lea    edi,[edi+0x0]               | NOP

```


# Q3 

Write asm for line: ` a = foo(a, 1, 2) ` , where `a` in `ebx`.


```

;;Assume each argument is 4 bytes and ret value of foo in `eax`.   

ASM:              ; ATT syntax               Rubrics

sub esp   12      ; allocate space stack
mov ebx   [esp]   ; set up args                +1
mov 1     [esp+4]                              +1
mov 2     [esp+8]                              +1
call foo          ; call function foo          +1
mov eax   ebx     ; put return value into ebx  +1
add esp   12      ; restore

You can also do:  

push 2
push 1
push ebx
call foo
mov eax ebx
```

Points:   5
Rubrics:  Each line is 1 point

# Q4 

Relocation: Find all lines needs relocation

```

  5 char buf[512];
  6 
  7 void
  8 wc(int fd, char *name)
  9 {
 10   int i, n;
 11   int l, w, c, inword;
 12 
 13   l = w = c = 0;
 14   inword = 0;
 15   while((n = read(fd, buf, sizeof(buf))) > 0){
 16     for(i=0; i<n; i++){
 17       c++;
 18       if(buf[i] == '\n')
 19         l++;
 20       if(strchr(" \r\t\n\v", buf[i]))
 21         inword = 0;
 22       else if(!inword){
 23         w++;
 24         inword = 1;
 25       }
 26     }
 27   }
 28   if(n < 0){
 29     printf(1, "wc: read error\n");
 30     exit();
 31   }
 32   printf(1, "%d %d %d %s\n", l, w, c, name);
 33 }
 
```

Lines: 15, 20, 29, 30, 32. (Calling functions in another file, global addr needs to be resolved)

Line:  18 (Global variabile buf)

Points:  20
Rubrics: Each line is 3 points. Attemp 2 points (wrote something down).  


# Q5

Typically, the stack is implemented as a continuous region of memory that is pre-allocated by the operating system when the process (or a thread) start executing. This, however, has certain limitations: i.e., some programs need tiny stacks, and some large, so there is no size that fits all. As a system designer you decide to support stacks of variable length. Specifically, on entry to each function you want to check the size of the current stack and if it is less then a certain constant (which you can define) allocate more memory for the stack. Of course the stack is no longer a single continuous region of memory, but a collection of regions that are somehow linked together (it's your choice how to link them). 

How do you need to change the prologue and epilogue of the function (i.e., what assembly code the compiler should generate on entry and return from a function for this idea to work). Explain your solution.

```

Approach 1:

Main a linkedin list of fixed-sized stack

uint32_t STACK_BASE ; Global var to keep track of current fix sized stack base 
Prologue:

if( (esp-STACK_BASE) < threshold ){
   mov eax esp              ; save old esp address
   mov esp <new_stack_addr> ; allocate new stack
   mov STACK_BASE esp       ; new current stack base       
   push eax                 ; push old esp address on top of the new stack
}

Epilogue:

if (esp+4 == STACK_BASE) {
   pop esp ;restore the old stack ( old stack address on bottom of the current stack)
}


Approach 2: 

Let ECX be a special register that keeps track of old esp value. (ECX is a seperate fix-sized stack
size of this special stack = MEM_SIZE / size_of(stack).) 

Prologue:

// If we need to allocate more space, we need to remember the old stack pointer address using a seperate stack
if( stack_space_left < threshold ){
   // Push to stack pointed by ECX
   sub ecx 4; 
   mov [ecx] esp;
   // now we can have another region of memory to be used as the new stack
   esp = new_stack_address; 
}

Epilogue:

// ECX stack is not empty, meaning the returned function stack is in a seperate region of memory, 
// we need to restore stack pointer from the ecx stack  
if( ECX != ECX_BASE ) {
   // Pop stack pointed by ECX and return old esp
   mov esp [ecx]
   add ecx 4 
} 

```


# Q6 

Address translation (Two solution due to error mentioned in class)


Translate VA: 0x1000

```
0x00001000 === 0b 0000 0000 0000 0000 0001 0000 0000 0000
                  |__________|___________| |____________|
                     PDE idx    PTE idx       Offset

PDE 0 : PPN=0x00001, PTE_P, PTE_U, PTE_W
PTE 1 : PPN=0x00006, PTE_P, PTE_U, PTE_W

0x00001000 --> 0x00006000
```

Translate VA: 0x2000

```
PTE 2 is not present, page fault. 
```

Points: 10 
Rubrics: 

- Indicate correct PDE entry idx: 3 points
- Indicate correct PTE entry idx: 3 points
- Indicate correct PA           : 3 points
- Solid Attempt                 : 1 points

# Q7


describing the page table as in the question above construct a page table that maps three pages at virtual addresses `0x8010 0000`, `0x8010 1000`, and `0x8010 2000` to physical addresses `0x0`, `0x8000 1000`, and `0xFFFF F000`. Use `x86-32`, `4KB` pages.


```
0x8010 0000 = 0b 1000 0000 0001 0000 0000 0000 0000 0000  --> 0x0 
                 |__________|___________| 
                    = 512      = 256 
0x8010 1000 = 0b 1000 0000 0001 0000 0001 0000 0000 0000  --> 0x8000 1000
                 |__________|___________| 
                    = 512      = 257
0x8010 2000 = 0b 1000 0000 0001 0000 0010 0000 0000 0000  --> 0xFFFF F000
                 |__________|___________| 
                    = 512      = 258 
                   

PDE 512: PPN=0x00001, PTE_P, PTE_U, PTE_W  // Note, PPN can be any address, but PDE idx must be 512
                                           // +1 for idx, +1 for PPN 
                                           
AT PA: 0x00001000 (PPN = 0x00001)          
PTE 256: PPN=0x00000, PTE_P, PTE_U, PTE_W  // +1 For idx, +1 for PPN 
PTE 257: PPN=0x80001, PTE_P, PTE_U, PTE_W  // Same above
PTE 258: PPN=0xFFFFF, PTE_P, PTE_U, PTE_W  // Same above
       
```

Points: 10
Rubrics: See above, 2 Points for attempt. 




