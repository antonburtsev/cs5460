#include "mmu.h"
#include "syscall.h"
#include "proc.h"
#include "console.h"

extern struct proc *current;
extern void exit();

// User code makes a system call with INT T_SYSCALL.
// System call number in %eax.
// Arguments on the stack, from the user call to the C
// library system call function. The saved user %esp points
// to a saved program counter, and then the first argument.
// Fetch the int at addr from the current process.
int
fetchint(uint addr, int *ip)
{

  // fix the bounds check (need to check for the lower bound of kernel memory)
  if(addr >= current->sz || addr+4 > current->sz)
    return -1;
  *ip = *(int*)(addr);
  return 0;
}

// Fetch the nul-terminated string at addr from the current process.
// Doesn't actually copy the string - just sets *pp to point at it.
// Returns length of string, not including nul.
int
fetchstr(uint addr, char **pp)
{
  char *s, *ep;

  if(addr >= current->sz)
    return -1;
  *pp = (char*)addr;
  ep = (char*)current->sz;
  for(s = *pp; s < ep; s++){
    if(*s == 0)
      return s - *pp;
  }
  return -1;
}

// Fetch the nth 32-bit system call argument.
int
argint(int n, int *ip)
{
  return fetchint(current->tf->esp + 4 + 4*n, ip);
}

// Fetch the nth word-sized system call argument as a pointer
// to a block of memory of size bytes.  Check that the pointer
// lies within the process address space.
int
argptr(int n, char **pp, int size)
{
  int i;
 
  if(argint(n, &i) < 0)
    return -1;
  if(size < 0 || (uint)i >= current->sz || (uint)i+size > current->sz)
    return -1;
  *pp = (char*)i;
  return 0;
}

// Fetch the nth word-sized system call argument as a string pointer.
// Check that the pointer is valid and the string is nul-terminated.
// (There is no shared writable memory, so the string can't change
// between this check and being used by the kernel.)
int
argstr(int n, char **pp)
{
  int addr;
  if(argint(n, &addr) < 0)
    return -1;
  return fetchstr(addr, pp);
}

int sys_print(void)
{

  char* s; 
  argstr(0, &s);
  printk(s);

  return 1;
}

int sys_exit(void)
{
  exit();
  return 1; // never reached
}

static int (*syscalls[])(void) = {
[SYS_print]    sys_print,
// TODO
};

void
syscall(struct trapframe *tf)
{
  uint num;

  current->tf = tf;

  num = current->tf->eax;
  if(num > 0 && num <= 2 && syscalls[num]) {

    // TODO: Invoke syscall correct syscall base on corresponding table entry
    // put the return result onto trap frame eax. 

  } else {
    current->tf->eax = -1;
  }
}
