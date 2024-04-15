; Context switch
;
;   void swtch(struct context **old, struct context *new);
; 
; Save the current registers on the stack, creating
; a struct context, and save its address in *old.
; Switch stacks to new and pop previously-saved registers.

section .text
global swtch

swtch:
  mov eax, [esp+4]    ; Load old context pointer from the stack
  mov edx, [esp+8]    ; Load new context pointer from the stack

  ; Save old callee-saved registers
  push ebp
  push ebx
  push esi
  push edi

  ; Switch stacks
  mov [eax], esp      ; Save old stack pointer in old context
  mov esp, edx        ; Load new stack pointer from new context

  ; Load new callee-saved registers
  pop edi
  pop esi
  pop ebx
  pop ebp
  ret                 ; Return
  