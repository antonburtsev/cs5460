global alltraps
global trapret


extern trap 
%define SEG_KCODE (1 << 3)
%define SEG_KDATA (2 << 3)

section .text
bits 32

; add alltraps, trapret, and vector32 here


