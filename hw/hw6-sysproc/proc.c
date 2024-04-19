#include "proc.h"
#include "mmu.h"
#include "elf.h"
#include "console.h"

struct context *kernel_context;
extern void trapret(void);
extern void swtch(struct context **, struct context *);
extern uint page_alloc();

extern void mmap(pde_t *pgdir, unsigned int va, unsigned int pa, unsigned int flag);

struct proc init_proc;
struct proc *current;

struct taskstate ts;

extern struct segdesc gdt[NSEGS];
extern char _binary_user_uprog_out_start[];

void *
memmove(void *dst, const void *src, uint n)
{
  const char *s;
  char *d;

  s = src;
  d = dst;
  if (s < d && s + n > d)
  {
    s += n;
    d += n;
    while (n-- > 0)
      *--d = *--s;
  }
  else
    while (n-- > 0)
      *d++ = *s++;

  return dst;
}

// memcpy exists to placate GCC.  Use memmove.
void *
memcpy(void *dst, const void *src, uint n)
{
  return memmove(dst, src, n);
}

void memzero(char *src, uint n)
{
  for (int i = 0; i < n; i++)
    src[i] = 0;
}

static inline void
ltr(ushort sel)
{
  asm volatile("ltr %0" : : "r"(sel));
}

// Switch to user kernel memory. Load TSS and PageTable.
void loadtss(struct proc *p)
{
  // disable interrupt
  cli();

  gdt[SEG_TSS] = SEG16(STS_T32A, &ts,
                       sizeof(ts) - 1, 0);
  gdt[SEG_TSS].s = 0;
  ts.ss0 = SEG_KDATA << 3;
  ts.esp0 = (uint)p->kstack + PGSIZE;
  // setting IOPL=0 in eflags *and* iomb beyond the tss segment limit
  // forbids I/O instructions (e.g., inb and outb) from user space
  ts.iomb = (ushort)0xFFFF;
  ltr(SEG_TSS << 3);

  // enable interrupt
  sti();
}

#define PAGESIZE 4096

void userinit(pde_t *pgdir)
{
  struct proc *p = &init_proc;

  struct elfhdr *elf;
  struct proghdr *ph, *eph;

  uint ppn;
  uint va = 0;

  elf = (struct elfhdr *)_binary_user_uprog_out_start; // this is where we linked our ELF in memory

  // Is this an ELF executable?
  if (elf->magic != ELF_MAGIC)
  {
    printk("wrong ELF magic\n");
    return;
  }

  // Load each program segment (ignores ph flags).
  ph = (struct proghdr *)((uchar *)elf + elf->phoff);
  eph = ph + elf->phnum;
  for (; ph < eph; ph++)
  {
    if (ph->type != ELF_PROG_LOAD)
    {
      continue;
    }

    va = PGROUNDDOWN(ph->vaddr);

    uint num_pg = ph->filesz / PAGESIZE;
    uint last_page_bytes = ph->filesz % PAGESIZE;
    uint src_addr = (uint)_binary_user_uprog_out_start + ph->off;

    for (int i = 0; i < num_pg; i++)
    {
      ppn = page_alloc();
      mmap(pgdir, va, ppn, PTE_U | PTE_W | PTE_P);
      memcpy((void *)ph->vaddr, (void *)src_addr, PAGESIZE);
      va += PGSIZE;
      src_addr += PAGESIZE;
      printk("mapped a user page\n");
    }

    if (last_page_bytes > 0)
    {
      ppn = page_alloc();
      mmap(pgdir, va, ppn, PTE_U | PTE_W | PTE_P);
      memcpy((void *)ph->vaddr, (void *)src_addr, last_page_bytes);
      va += PGSIZE;
      src_addr += last_page_bytes;
      printk("mapped a user page\n");
    }
  }


  // SECOND PART !

  p->pgdir = pgdir;
  p->kstack = (char *)page_alloc();
  memzero(p->kstack, PAGESIZE);

  char *sp;
  sp = p->kstack + PGSIZE;


  // TODO: Initilize kernel stack
  // sp is your kernel stack pointer. 
  // put process trap frame and context on stack

  // TODO: uncomment below to fill in ?s
  // p->tf->eip = ?
  // p->context->eip = ?

  p->tf->cs = (SEG_UCODE << 3) | DPL_USER;
  p->tf->ds = (SEG_UDATA << 3) | DPL_USER;
  p->tf->es = p->tf->ds;
  p->tf->ss = p->tf->ds;
  p->tf->eflags = FL_IF;

  // Allocate a page for the user stack
  ppn = page_alloc();
  mmap(pgdir, va, ppn, PTE_U | PTE_W | PTE_P);

  p->tf->esp = va + PGSIZE - 4; // User stack, we don't pass any arguments

  va += PGSIZE;
  p->sz = va;

  current = p;
};

void run_current()
{
  loadtss(current);
  swtch(&kernel_context, current->context);
}

void exit(void)
{
  swtch(&current->context, kernel_context);
}
