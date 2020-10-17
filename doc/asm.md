# Assembly examples

### push instruction

```asm
EBX contains 0x55, ESP is 0x1004:

stack (grows down):
0x1008:  |     ....               |
0x1004:  |     0xF00              |    < --   ESP

you execute: 
      push EBX

ESP is now 0x1000
0x1008:  |     ....               |
0x1004:  |     0xF00              |   
0x1000:  |     0x55               |    < -   ESP

```

### calling a function and storing local variables

```asm
EBX contains 0x55, EBP is 0x1100, ESP is 0x1004:

stack (grows down):
0x1008:  |     ....               |    < --   ESP

--------------------------
caller: 
      push ebp
      mov ebp, esp
      sub esp, 12 ; local variables
      mov [ebp - 4], 10
      mov [ebp - 8], 5
      mov [ebp - 12], 2

you execute:
      call caller

ESP is now 0x0FF4 and EBP is now 0x1100
0x1008:  |     ....               |
0x1004:  |     0xbc1293C          |   
0x1000:  |     0x1100             |    < - EBP
0x0FFC:  |     10                 |    
0x0FF8:  |     5                  |   
0x0FF4:  |     2                  |    < - ESP

```

### caller function calls "callee" function

```asm
ESP is 0x0FF4 and EBP is now 0x1000
0x1008:  |     ....               |
0x1004:  |     0xF00              |   
0x1000:  |     0x1100             |    < - EBP
0x0FFC:  |     10                 |    
0x0FF8:  |     5                  |   
0x0FF4:  |     2                  |    < - ESP

callee:
      push ebp
      mov ebp, esp
      mov eax, 0
      add eax, [ebp + 8]
caller: 
      call callee

ESP and EBP is at 0x0FEC
register EAX now contains 2
0x1008:  |     ....               |
0x1004:  |     0xF00              |   
0x1000:  |     0x1100             |    
0x0FFC:  |     10                 |    
0x0FF8:  |     5                  |   
0x0FF4:  |     2                  |
0x0FF0:  |address of instruction after "call"|
0x0FEC:  |     0x1000             |    < - EBP, ESP

```

### imagine we just executed "add eax, [ebp + 8]", what happens next?

```asm
ESP and EBP is are 0x0FEC
register EAX now contains 2
0x1008:  |     ....               |
0x1004:  |     0xF00              |   
0x1000:  |     0x1100             |    
0x0FFC:  |     10                 |    
0x0FF8:  |     5                  |   
0x0FF4:  |     2                  |
0x0FF0:  |address of instruction after "call"|
0x0FEC:  |     0x1000             |    < - EBP, ESP

added ret code 
callee:
      push ebp
      mov ebp, esp
      mov eax, 0
      add eax, [ebp + 8]
      pop ebp
      ret
main: 
      call callee
      push eax

ESP is now 0x0FF0 and EBP is now 0x1000
register EAX still contains 2
0x1008:  |     ....               |
0x1004:  |     0xF00              |   
0x1000:  |     0x1100             |  < - EBP  
0x0FFC:  |     10                 |    
0x0FF8:  |     5                  |   
0x0FF4:  |     2                  |
0x0FF0:  |     2                  | < - ESP

```
