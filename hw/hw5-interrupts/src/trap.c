#include "mmu.h"
#include "traps.h"
#include "console.h"

// Define Interrupt descriptor table
struct gatedesc idt[256];

extern void vector32(); // Address interrupt is going to jump to

// Call me! 
void initpics() {
  lapicinit(); 
  picinit();
  ioapicinit();
}

// TODO: initilize IDT with vector32 and load it with lidt, and enable interrupt
void
tvinit(void)
{
  // Initilize IDT with Timer Interrupt
  // Load IDT using lidt
  // Enable interrupt
}

//TODO: Handle your interrupts, implement a timer interrupt 
void
trap(struct trapframe *tf)
{
}
