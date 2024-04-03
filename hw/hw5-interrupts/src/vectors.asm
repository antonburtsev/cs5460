global alltraps
global trapret

extern trap 
%define SEG_KCODE (1 << 3)
%define SEG_KDATA (2 << 3)

section .text
bits 32

; TODO:

; Build trap frame, switch to Kernel data, and call trap function
alltraps:


; Implement Return by restoring the trap frame
trapret:


; Set up arguments, Jmp to alltraps
global vector32
vector32: