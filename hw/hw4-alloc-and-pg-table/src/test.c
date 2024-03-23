#include "mmu.h"

extern char end[];
void page_free(unsigned int);
void mmap(pte_t*, unsigned int, unsigned int, unsigned int);

static inline uint
read_cr0(void)
{
    uint val;
    asm volatile("movl %%cr0,%0" : "=r" (val));
    return val;
}

static inline void
write_cr0(uint val)
{
    asm volatile("movl %0,%%cr0" : : "r" (val));
}

static inline void
write_cr3(uint val)
{
    asm volatile("movl %0,%%cr3" : : "r" (val));
}

void enable_paging(pte_t* pgdir)
{
    write_cr3((uint)pgdir);
    int cr0 = read_cr0();
    cr0 |= CR0_PG;
    write_cr0(cr0);
}

void disable_paging() 
{
    int cr0 = read_cr0();
    cr0 &= 0x7FFFFFFF;
    write_cr0(cr0);
}

__attribute__((__aligned__(PGSIZE)))
static pte_t pgdir[NPDENTRIES];

#define MAX_ADDR 0x2000000

void test() 
{
    disable_paging();
    
    for(uint page=PGROUNDUP((uint) end); page<=MAX_ADDR; page+=PGSIZE)
        page_free(page);
  
    uint flag = PTE_P + PTE_W;
    for(unsigned int va=0; va<=MAX_ADDR;va+=PGSIZE) 
      mmap(pgdir, va, va, flag);

    enable_paging(pgdir);
}
