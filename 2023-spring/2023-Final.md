Final exam key

### Q1.1 Stack layout at printf


Imagine you break right on the printf() line below in the following xv6 program (specifically
right on the call instruction that is about to call the printf function inside the bar function):

```
int bar(int x, int y) {
  printf(1, "x:%d, y:%d\n", x, y);
  return x;
}
int foo(int a, int b, int c) {
  return bar(a + b, c);
}
main() {
  foo(1, 2, 3);
  exit(0);
}
```

Please draw the stack and explain every value on the stack. Assume that compiler does not
use stack alignment, but maintains stack frames. You can make up any values for the return
addresses.


Answer:
``` 
Stack layout: 
---
argv     
argc
0xffffffff ; fake ret
ebp     ; main
3       ; c                    
2       ; b                   
1       ; a                   
retaddr ; exit(0)             
ebp     ; foo                 
3    ; y                      
3    ; x                      
retaddr ;ret bar              
ebp  ; bar                     
3    ;  y                      
3    ;  x                      
addr ;  addr of "x:%d, y:%d\n" 
1                                        
--- 
```

Note: printf function in xv6 doesn't use va_list. It simply just push 
the arguments on the stack like a normal function.  

### Q2.1 Proces Memory 

Draw and explain organization of the process address space in the xv6 system. 
Be specific. ASCII drawing that you can copy as a skeleton for your answer. 

Answer: 
```
0                             (0x80000000)             (0xFFFFFFFF)
-----------------------------------|-----------------------|
| Text&Data | Guard | Stack | Heap |        Kernel         |
-----------------------------------------------------------| 
```

### Q2.2 Possible outputs

Alice executes the following xv6 program

```
  1 #include "types.h"
  2 #include "stat.h"
  3 #include "user.h"
  4 
  5 int
  6 main(int argc, char *argv[])
  7 {
  8 
  9   char *msg = "bar\n";
 10   int pid = fork();
 11   if (pid)
 12     msg = "foo\n";
 13   else
 14     msg = "baz\n";
 15   write(1, msg, 4);
 16   exit();
 17 }
```

What are all possible outputs of this program? Explain your answer.

Answer: 
```
1. foo\nbaz\n ( Parent writes first, Child writes second)  
2. baz\nfoo\n ( Child writes first, Parent writes second)
3. foo\n (Fork fails)
```

### Q2.3 Bob's argument
Bob argues with Alice that there will never be an output with interleaving characters? E.g., “fbaozo” Is he correct? Explain your answer?

Answer: 
```
Yes. Bob is correct.

Explanation:
The write() call follows the following call sequence, write() -> filewrite() -> consolewrite(). 
The function consolewrite() acquires the console lock such that only one thread can be writing to the console. 
So, there won't be any interleaving characters


```

### Q2.4 Alice's experiment

Alice continues experimenting with xv6. She writes the following xv6 program to test recursive invocation of functions. 

```
  1 #include "types.h"
  2 #include "stat.h"
  3 #include "user.h"
  4 
  5 
  6 int foo(char *p) {
  7   write(1, "hello\n", 6);
  8   foo(p);
  9   return 0;
 10 }
 11 
 12 int
 13 main(int argc, char *argv[])
 14 {
 15    char a[4096];
 16    foo(a);
 17    exit();
 18 }
```

How many times Alice will see "hello" on the screen? Explain your answer.

Answer: 
```
Alice should see 0 times of "hello". This is because c will allocate local array on the stack here, which is the size of the user stack, so no space left for foo. ( program will trap when foo is invoked because it will hit the guardpage). 
```

### Q2.5 Init process

Alice is running an xv6 system with two CPUs. 
Is it possible for the init() process which is 
created on the first CPU to run on the second CPU 
at some point in time? Be specific, explain why this may or may not happen.

Answer:
```
Yes, this is possible. Xv6 runs a scheduler on each physical CPU. Each scheduler goes through the table of processes and pics the one that can run (i.e., is in the RUNNABLE state). Sooner or later the init process will be picked up by the scheduler of the second CPU, will be context switched into, and will run.
```

### Q3.1 Explain context switch

Explain how xv6 switches from one process to another? Be specific, refer to the source code and function that implement the context switch.

Answer: 
Here is a ASCII diagram with function calls: 

```
                   --> swtch -> sched -> yield -> alltrap  
                   |
                   |
p1 --->  scheduler  --->   p2
   |
   |
   --> alltrap (timer) -> yield -> sched -> swtch  

```

### Q3.2 Why save EBX twice

During the context switch the register EBX gets saved twice, 
once by the popal instruction in the alltraps() function 
and second in the swtch() function. Can you explain why do we need to save it
twice?

Answer:
```
we save EBX because

alltrap(): 
We need to return back to user space.
EBX value of the user-process is saved on the trapframe inside the 
proc->tf->ebx field of the trapframe data structure.

swtch(): 
1. We need to context switch to another process in kernel
2. EBX happens to be a callee-saved register. 

Then since EBX is callee saved register, the kernel cannot assume that
it's value will be presumed while some other process is switched to run 
on the CPU. Hence xv6 pushes it on the stack as part of the context data structure
(proc->context->$ebx)
```

### Q3.3 explain context 


Explain the role of the context field in the proc data structure in xv6.
How is it used during the context switch?

Answer: 

It is used to save/switch the contexts between two process. 

```
.globl swtch
swtch:
  movl 4(%esp), %eax  ;old context
  movl 8(%esp), %edx  ;new context

  ; ESP still points to old context
  ; we are saving value onto old context

  # Save old callee-saved registers
  pushl %ebp  
  pushl %ebx
  pushl %esi
  pushl %edi

  ; We switch the stack aka. context

  # Switch stacks
  movl %esp, (%eax) ;save esp onto old context 
  movl %edx, %esp   ;now switch stack/new context

  ; ESP points to new context
  ; we are getting all the value of the new context 

  # Load new callee-saved registers
  popl %edi
  popl %esi
  popl %ebx
  popl %ebp
  ret
```


### Q4.1: Explain ln

Can you explain what operations the xv6 file system performs 
when Bob invokes the following xv6 command

```
ln README foobar
```


Answer: Look into sys_link and dirlink. 
```
  1. Look up README inode, update its link
  2. Dirlink, which associates name with a inode, and put it in a directory. 
  so, in this case Dirlink("foobar", README_INODE, "./");
```

### Q4.2: Alice Experiment 2 ( sure does love to experiment). 


After taking the last quiz in cs5460 Alice knows that every directory in xv6 can have the maximum of 4480 entries (files or sub-directories). Being an aspiring kernel hacker she decides to verify if this answer is correct. She writes a small xv6 program that creates 8000 links to the same README file. I.e., her program executes the following commands: 

```
ln README file1
ln README file2
ln README file3
...
```
But as a combination of `fork()` and `exec()` system calls. 

```
int
main(int argc, char *argv[])
{

  char *execargv[] = { "ln", "README", cmd, 0 };

  for(int i = 1; i < 8000; i++){
    mkfilename(cmd + 4, i); 
   
    printf(1, "ln README %s\n", cmd);
    int pid = fork();
    if(pid == 0)  
        exec("ln", execargv);
    else
        wait();
           
  }
  exit();
}
```

She is very happy that the program runs creating symbolic links one after another. She rushes to explain her experiment to Bob. But while watching her program running she notices that it becomes progressively slower, so slow that even after 15 minutes it's still on file 2033, and creating every new link takes several seconds. Can you explain why performance degrades so sharply (be specific and use references to xv6 source code).

Answer: 

The size of the buffer cache is not big enough to hold all directory entries.
so ln command need to perform linear scan to find empty inodes in a directory, as 
directory size grow, it will get slower.

Look into dirlink: 
```
int
dirlink(struct inode *dp, char *name, uint inum)
{
  int off;
  struct dirent de;
  struct inode *ip;

  // Check that name is not present.
  if((ip = dirlookup(dp, name, 0)) != 0){
    iput(ip);
    return -1;
  }

------- Performance Issue ------------
;; Linear scan 
  // Look for an empty dirent.
  for(off = 0; off < dp->size; off += sizeof(de)){
    if(readi(dp, (char*)&de, off, sizeof(de)) != sizeof(de))
      panic("dirlink read");
    if(de.inum == 0)
      break;
  }
------- Performance Issue ------------

  strncpy(de.name, name, DIRSIZ);
  de.inum = inum;
  if(writei(dp, (char*)&de, off, sizeof(de)) != sizeof(de))
    panic("dirlink");

  return 0;
}
```












