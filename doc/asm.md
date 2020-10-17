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
