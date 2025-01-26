# Final

## 1

```
int main(int argc, char* argv[]) {
  int fd, n;
  char buf[128];

  if ((fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC)) < 0) {
    printf(2, "tee: cannot open %s\n", argv[1]);
    exit();
  }
  
  while ((n = read(0, buf, sizeof(buf))) > 0) {
    if (write(1, buf, n) != n) {
      printf(2, "tee: write error\n");
      exit();
    }

    if (write(fd, buf, n) != n) {
      printf(2, "tee: write error\n");
      exit();
    }
  }

  if (n < 0){
    printf(2, "tee: read error\n");
    exit();
  }
  
  close(fd);
  exit();
}
```

## 2
```
pushl $0 
```

because that is the first line of code in the system call trap handler, where kernel mode is entered. printf internally calls a system call which cause a trap (int 0x40), which according to vectors.pl will generate pushl $0 as first line of code for the trap handler

## 3

### 3.1
```
movl %esp, (%eax)
```

The kernel ESP register is saved inside the swtch() function as a pointer to the context
of currently running process. This line saves ESP into proc->context making this pointer point to the top of the stack of the process that was running before the swtch() was invoked.

### 3.2

The user ESP pointer is saved as part of the interrupt transition (i.e., the hardware pushes
5 things on the stack, when interrupt arrives and one of them is ESP).

### 3.3

The kernel EAX register is a caller-saved register, if the function that calls swtch() uses
this register after the swtch() invocation, it will push the register on the stack before
invoking swtch().

### 3.4

The function alltrap calls pusha. The eax is stored in the trapframe. 
3310 pushal (this is the line of code)

## 4

### 4.1
Yes. When there is only a single CPU,  one task will run at a time, so there will not be the case where one process can be run at the same time unless the single CPU is multi-threaded.

### 4.2
No. There will be race condition when two CPU tries to make a process runnable and execute the same program at the same time.

## 5
0x00000005, 0x00400005, 0x00801005

## 6
### 6.1
12288

proc->sz is the size of the user memory (in bytes). At the very least, 1 page will be allocated for the text and data sections (and this minimal process will fit in only 1 page). There will be a single guard page. There will be a single page for the user stack. 3 pages, each 4096 bytes = 12288 bytes

### 6.2

0x7E000000

reason 1) I looked at the xv6 source code and found:
1826  if (P2V(PHYSTOP) > (void*)DEVSPACE)
1827    panic("PHYSTOP too high");
// and DEVSPACE is defined as 0xFE000000 (which is physical 0x7E000000)

reason 2) The virtual kernel address space is 2GB (the 2GB to 4GB section of virtual memory). Some of the BIOS region/device region of kernel memory starts at virtual address 0xFE000000 (DEVSPACE above) or physical address 0x7E000000. Any physical memory above 0x7E000000 cannot be used because the region 0x7E000000 to 2GB (2GB because the kernel is 2GB) is reserved for the devices. 

## 7
### 7.1
Block 2 contains the log header. It's written twice because it's the records of the transaction are written.
The first "write 2" signifies the transaction being recorded. This will write integers 2 (for 2 blocks being written) and 32 and 59 (the block numbers to be written).
The second "write 2" signifies the transaction being completed. It will write an integer 0 (to indicate no more blocks to write).

### 7.2

Yes, foo will be visible on disk after reboot.
The logging mechanism ensures a transaction is either done entirely or not at all. Once the log header is first written (indicated by the first "write 2"), the transaction is logged. Then, no matter what happens, even if there's a crash, blocks 32 and 59 will be written because those block numbers were logged.

### 7.3

4479 times

A directory data block has 512 bytes worth of entry data, and a directory entry is 16 bytes. That means there are 32 entires per directory data block. A directory inode has the 12 direct addresses and 128 indirect addresses, so there are 32*140 = 4480 possible directory entries. Assuming README takes up one slot in the current directory already, and there's nothing else in the directory, that leaves 4479 available entries.
