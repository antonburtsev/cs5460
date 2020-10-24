; file: nasm_rc.asm
section    .text
    ; The _start symbol must be declared for the linker (ld)
    global _start

_start:
    ; Execute sys_exit call. Argument: status -> ebx
    mov     eax, 1  ; system call 1: sys_exit
    mov     ebx, 42 ; pass arguments to sys_exit
    int     0x80    ; call into kernel
