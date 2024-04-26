# Final  

## 1 

### 1.1

No, because there is no mapped page in either of the page tables that maps to the physical page zero (there are technically entries that map to PPN 0 but they don't have the proper flags set).

### 1.2

5, 6

### 1.3

0x0 - 0xfff  
0x1000 - 0x1fff  
0x400000 - 0x400fff  
0x401000 - 0x401fff  
0x800000 - 0x800fff  
0x801000 - 0x801fff  

### 1.4

pgdir (at PPN 0)  
PDE 0: PPN and flags are all zeroes  
PDE 1: PPN=0x1, PTE_P, PTE_U, PTE_W  
... all other PDEs are zero  

pgtab (at PPN 1)  
PTE 0: PPN=0x0, PTE_P, PTE_U, PTE_W  
PTE 1: PPN=0x1, PTE_P, PTE_U, PTE_W  
PTE 2: PPN=0x2, PTE_P, PTE_U, PTE_W  
... for all other PTEs, the PPN is equal to the PTE number  

## 2

### 2.1

You would have to change lines 6665 and 6667. Right now, line 6665 allocates 2 pages (one guard page and one user stack page). You have have to change 2*PGSIZE to 3*PGSIZE in order to allocate 3 pages instead (one guard page and two user stack pages). Line 6667 clears the user bit on the guard page, but since you allocated 3 pages instead of 2, you would have to change 2*PGSIZE to 3*PGSIZE on this line as well to make sure the correct page is the guard page.

### 2.2
2113929216 kb, around 2 gb
xv6 will panic if P2V(PHYSTOP) > (void*)DEVSPACE
P2V adds KERNBASE to PHYSTOP
Subtract KERNBASE from DEVSPACE and you get the highest possible PHYSTOP

## 3
### 3.1
Yes  
Code: Line 3373: SETGATE(idt[T_SYSCALL], 1, SEG_KCODE<<3, vectors[T_SYSCALL], DPL_USER);  
Explanation: The second argument being 1 indicates that the gate is a "trap" gate. When entering a trap gate, the IF flag won't be cleared, so interrupts may still happen.  

### 3.2
No    
Line 3372: SETGATE(idt[i], 0, SEG_KCODE<<3, vectors[i], 0);  
For all gates except for T_SYSCALL, the second argument to SETGATE is 0, meaning that the gate is an interrupt gate. For interrupt gates, the IF_FLAG is cleared, so no interrupts may happen while their handler is running.

### 3.3
Lines 2582-2583 aren't lines of "code" (since they don't become assembly instructions).
Line 2584 calls myproc(), which has a pushcli() call in it. Thus, part of "line 2584" can be interrupted and part of it can't.
Line 2587 calls allocproc(), which calls acquire(), which calls pushcli(). Once again, part of "line 2587" can be interrupted and part of it can't.
Lines 2588-2592 can all be interrupted.
Line 2593 calls kfree(), which calls acquire(), which calls pushcli(). Part of "line 2587" can be interrupted and part of it can't.
Lines 2594-2606 can all be interrupted.
Line 2607 calls filedup(), which calls acquire(), which calls pushcli(). Part of "line 2607" can be interrupted and part of it can't.
Line 2608 calls idup(), which calls acquire(), which calls pushcli(). Part of "line 2608" can be interrupted and part of it can't.
Lines 2610-2612 can all be interrupted.
Line 2614 calls acquire(), which calls pushcli(). We have to wait until line 2618 until popcli() is called. Thus, lines 2614-2618 cannot be interrupted.
Line 2620 can be interrupted.

## 4

### 4.1

If she removes that code from the line, the code will no longer check if i is a valid address within the process space. Thus, it would be possible for the process to attempt to access an address outside of its own memory when determining system call arguments.

### 4.2

First you'd pass the first 3 arguments to the EDI, ESI, and EDX registers, respectively, after saving their old values on the user stack. You'd then save all the other system call arguments past the 3rd one onto the user stack as well. You'd then have to change the code for argint, argfd, and argptr for whatever arg number you're getting:
for example, for argint:
int argint(int n, int *ip)
{
  if (n == 0)
     return myproc()->tf->edi;
  else if (n == 1)
     return myproc()->tf->esi;
  else if (n == 2)
     return myproc()->tf->edx;
  else
     return fetchint((myproc()->tf->esp) + 4 + 4*(n - 3), ip);
}
After you're done with the system call you'd have to return the EDI, ESI, and EDX registers to their original values.

## 5
### 5.1

The role of acquiring the lock to the condition is to avoid the lost wakeup problem. If there were no lock around the condition check and the sleep() call, the following situation might happen. First, the loop checks that the pipe is empty and decides it needs to go to sleep. However, before it actually calls sleep(), another other process running pipewrite writes some data, calls wakeup() on the piperead process, and goes to sleep itself. Then, the piperead process actually goes to sleep(). Now, both the piperead and pipewrite process are sleeping, and since they rely solely on each other to wake them up, they'll both never wake up, resulting in a deadlock.

This lock makes sure that checking the condition and calling sleep() happens atomically, making sure this situation will never happen.

### 5.2
Some programs might run slower.

### 5.3
As explained in Q5.1, a lock to check the condition in the while loop around sleep() must be acquired before sleep() is called. However, this same lock must be released in sleep() before the process actually goes to sleep. If p->lock were not released, then the pipewrite process would never be able to write anything to the pipe (since it needs to acquire the lock to modify the pipe), so the piperead process would never wakeup, resulting in deadlock.

### 5.4
No. The return value of piperead measures how many bytes were read in this call of piperead, not how many bytes were read in total (that value would be p->nread). In a single call of piperead, it's impossible to read more than PIPESIZE because the pipe only holds PIPESIZE bytes in total. pipewrite only writes until nwrite == nread + PIPESIZE at most, and since you start reading from the initial value of nread, you'll never read more than PIPESIZE bytes.
Graded
