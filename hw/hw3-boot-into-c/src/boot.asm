global start
extern main

%define SEG_KCODE (1 << 3)
%define SEG_KDATA (2 << 3)

section .text

bits 32    ; By default, GRUB sets us to 32-bit mode.
start:

    ; Print `Hello, world!` to VGA screen
    mov word [0xb8000], 0x0248 ; H
    mov word [0xb8002], 0x0265 ; e
    mov word [0xb8004], 0x026c ; l
    mov word [0xb8006], 0x026c ; l
    mov word [0xb8008], 0x026f ; o
    mov word [0xb800a], 0x022c ; ,
    mov word [0xb800c], 0x0220 ;
    mov word [0xb800e], 0x0277 ; w
    mov word [0xb8010], 0x026f ; o
    mov word [0xb8012], 0x0272 ; r
    mov word [0xb8014], 0x026c ; l
    mov word [0xb8016], 0x0264 ; d
    mov word [0xb8018], 0x0221 ; !


    call main
    hlt ; Halt the processor.


