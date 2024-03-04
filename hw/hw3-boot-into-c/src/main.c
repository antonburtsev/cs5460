#include "console.h"
#include "mmu.h"



static inline void halt(void)
{
    asm volatile("hlt" : : );
}

static inline void
write_gdt(struct segdesc *p, int size)
{
    volatile ushort pd[3];

    pd[0] = size-1;
    pd[1] = (uint)p;
    pd[2] = (uint)p >> 16;

    asm volatile("lgdt (%0)" : : "r" (pd));
}

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


int main(void)
{
    int i; 
    int sum = 0;

    // Initialize the page table here

    // Initialize the console
    uartinit(); 

    printk("Hello from C\n");

    // This test code touches 32 pages in the range 0 to 8MB
    for (i = 0; i < 64; i++) {
        int *p = (int *)(i * 4096 * 32);
        sum += *p; 
                
        printk("page\n"); 
    }
    halt(); 
    return sum; 
}


