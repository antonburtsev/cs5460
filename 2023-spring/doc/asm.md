# Assembly examples

In all examples we use Intel x86 32bit assembly. Stack grows down towards 0x0. 

## push instruction

**Before:**

`EBX` contains `0x55`, `ESP` is `0x1004`:

Stack (grows down):

```asm
0x1008:  |     ....               |
0x1004:  |     0xF00              |    < --   ESP
```

**Execute:**: 

```asm
      push EBX
```

**After:**

```asm
ESP is now 0x1000
0x1008:  |     ....               |
0x1004:  |     0xF00              |   
0x1000:  |     0x55               |    < -   ESP

```
## pop instruction

**Before:**

`EAX` contains `0x0`, `ESP` is `0x1000`:

```asm
stack (grows down):
0x1008:  |     ....               |
0x1004:  |     0xF00              |   
0x1000:  |     0x55               |    < -   ESP
```

**Execute**: 

```
      pop EAX
```

**After**: 


```asm
ESP is now 0x1004
0x1008:  |     ....               |
0x1004:  |     0xF00              |    < -   ESP
0x1000:  |     0x55               |    ; note, stack memory still contains 0x55
...
```

`EAX` is now `0x55`

## call instruction

**Before:**

`ESP` is `0x1008`:

stack (grows down):
```asm
0x1008:  |     0xABC0              |    < --   ESP
```

The `foo()` looks like this, note, it does not maintain the stack frame, just returns 
5 via `EAX`

```asm
foo: 
     mov EAX, 5
     ret
``` 
     
**Execute:**

You execute the `call foo` instruction but nothing inside `foo()`:

```asm
0x1230      call foo   <--  EIP
0x1235      ....
```

**After**:

`ESP` is now `0x1004`

```asm
0x1008:  |     0xABC0             |
0x1004:  |     0x1235             |    < -- ESP 
...           
```

## ret instruction

**Before**:

`ESP` is `0x1004`:

stack (grows down):
```asm
0x1008:  |     0xABC0             |
0x1004:  |     0x1235             |    < -- ESP 
...           
```

The `foo()` looks like this, note, it does not maintain the stack frame, just returns 
5 via `EAX`

```asm
foo: 
     mov EAX, 5
     ret          <--- EIP
``` 

**Execute**:
     
You execute the `ret` instruction inside `foo()`:

**After:**

`ESP` is now `0x1008`

```asm
0x1008:  |     0xABC0             |    < -- ESP
0x1004:  |     0x1235             |    # note, this memory still contains "return" address 
...           
```

## Call a function, maintain the stack frame, and allocate space for local variables

**Before:**

`EBX` contains `0x55`, `EBP` is `0x1100`, `ESP` is `0x1008`:

stack (grows down):
```asm
0x1008:  |     0xABC0              |    < --   ESP
```

```asm
foo: 
      push ebp        ; save old frame pointer 
      mov ebp, esp    ; set new frame pointer
      sub esp, 12     ; allocate space for local variables (3 integers (4 byte values))
      mov [ebp - 4], 10 ; initialize variables to 10, 5, and 2
      mov [ebp - 8], 5
      mov [ebp - 12], 2
      ...             <-- Execute up to here
```
      
**Execute**:

You execute the `call foo` instruction and then 5 instructions inside foo (see above):
```asm
0x1230      call foo < -- EIP
0x1235      ....
```

**After**: 

`ESP` is now `0x0FF4` and `EBP` is now `0x1100`

```asm
0x1008:  |     0xABC0             |
0x1004:  |     0x1235             |   
0x1000:  |     0x1100             |    < - EBP
0x0FFC:  |     10                 |    
0x0FF8:  |     5                  |   
0x0FF4:  |     2                  |    < - ESP
...           
```

## Return local variable 

**Before:**

`ESP` is now `0x0FF4` and `EBP` is now `0x1100`

```asm
0x1008:  |     0xABC0             |
0x1004:  |     0x1235             |   
0x1000:  |     0x1100             |    < - EBP
0x0FFC:  |     10                 |    
0x0FF8:  |     5                  |   
0x0FF4:  |     2                  |    < - ESP
...           
```

**Execute**:

```asm
foo: 
      push ebp        ; save old frame pointer 
      mov ebp, esp    ; set new frame pointer
      sub esp, 12     ; allocate space for local variables (3 integers (4 byte values))
      mov [ebp - 4], 10 ; initialize variables to 10, 5, and 2
      mov [ebp - 8], 5
      mov [ebp - 12], 2
      mov eax, [ebp - 4]  <-- EIP
      mov esp, ebp    ; restore stack to state on entry
      pop ebp         ; restore ebp
      ret             ; return

```
      
### Step by step

Lets take a look at how stack and register change after each instruction 

**Execute**

```asm
      mov eax, [ebp - 4]  <-- EIP
```

**After**

`EAX` is now 10

**Execute**

```asm
      mov esp, ebp    ; restore stack to state on entry
```

**After**

`ESP` and `EBP` point to `0x1000`

```asm
0x1008:  |     0xABC0             |
0x1004:  |     0x1235             |   
0x1000:  |     0x1100             |    < - EBP, ESP
0x0FFC:  |     10                 |    
0x0FF8:  |     5                  |   
0x0FF4:  |     2                  |    
...           
```

**Execute**

```asm
      pop ebp         ; restore ebp
```

**After**

`ESP` is now `0x1004`

```asm
0x1008:  |     0xABC0             |
0x1004:  |     0x1235             |    < -- ESP 
0x1000:  |     0x1100             |    
0x0FFC:  |     10                 |    
0x0FF8:  |     5                  |   
0x0FF4:  |     2                  |    
...           
```

**Execute**

```asm
      ret
```

**After**

`ESP` is now `0x1008`

```asm
0x1008:  |     0xABC0             |    < -- ESP
0x1004:  |     0x1235             |     
0x1000:  |     0x1100             |    
0x0FFC:  |     10                 |    
0x0FF8:  |     5                  |   
0x0FF4:  |     2                  |    
...           
```

`EIP` is now at `0x1235` and we continue execution after the call

```asm
0x1230      call foo 
0x1235      ....      < -- EIP
```








