#include "mmu.h"
#include "traps.h"
#include "console.h"

// Define Interrupt descriptor table
struct gatedesc idt[256];

extern void vector32();

void initpics() {
  lapicinit(); 
  picinit();
  ioapicinit();
}

// TODO: initilize IDT with vector32 and load it with lidt
void
tvinit(void)
{
}

//TODO: Handle your interrupts, implement a timer interrupt 
//Hint: remeber T_IRQ0 (32) is the first user interrupt  
void
trap(struct trapframe *tf)
{
}
