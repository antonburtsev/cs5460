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

void
tvinit(void)
{
}


void
trap(struct trapframe *tf)
{
}
