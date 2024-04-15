#ifndef _PROC_H_
#define _PROC_H_

#include "traps.h"
#include "mmu.h"


struct context
{
  uint edi;
  uint esi;
  uint ebx;
  uint ebp;
  uint eip;
};

enum procstate
{
  UNUSED,
  RUNNABLE,
  RUNNING,
  DONE
};

struct proc
{
  uint sz;                 // Top address of this process
  char *kstack;            // Bottom of kernel stack for this process
  pde_t *pgdir;            // Page table
  enum procstate state;    // Process state
  struct context *context; // swtch() here to run process
  struct trapframe *tf;    // Trap frame for current syscall
};

#endif
