%define T_SYSCALL 64
%define SYS_print 1 ; This need to be same as syscall.h
%define SYS_exit  2 ; This need to be same as syscall.h



section .text
bits 32

global print
print:
  mov eax, SYS_print
  int T_SYSCALL
  ret

;  TODO
