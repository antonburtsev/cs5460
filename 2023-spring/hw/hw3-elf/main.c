#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>

#include <sys/mman.h>


#include <sys/types.h>
#include <unistd.h>

// Format of an ELF executable file

#define ELF_MAGIC 0x464C457FU  // "\x7FELF" in little endian

// File header
struct elfhdr {
  unsigned int magic;  // must equal ELF_MAGIC
  unsigned char elf[12];
  unsigned short type;
  unsigned short machine;
  unsigned int version;
  unsigned int entry;
  unsigned int phoff;
  unsigned int shoff;
  unsigned int flags;
  unsigned short ehsize;
  unsigned short phentsize;
  unsigned short phnum;
  unsigned short shentsize;
  unsigned short shnum;
  unsigned short shstrndx;
};

// Program section header
struct proghdr {
  unsigned int type;
  unsigned int off;
  unsigned int vaddr;
  unsigned int paddr;
  unsigned int filesz;
  unsigned int memsz;
  unsigned int flags;
  unsigned int align;
};

// Values for Proghdr type
#define ELF_PROG_LOAD           1

// Flag bits for Proghdr flags
#define ELF_PROG_FLAG_EXEC      1
#define ELF_PROG_FLAG_WRITE     2
#define ELF_PROG_FLAG_READ      4

int main(int argc, char* argv[]) {
    struct elfhdr elf;
    struct proghdr ph;
    int (*sum)(int a, int b);
    void *entry = NULL;
    int ret; 

    /* Add your ELF loading code here */
    
    int file;
    file = open("./elf", O_RDONLY);

    if(file == 0 ) {
      printf("file cannot be opened");
      return -1;
    }
    
    read(file, &elf, sizeof(struct elfhdr));
    
    //read MAGIC
    // lseek(file, elf.phoff, SEEK_SET);

    void *src;
      
    int entries = elf.phnum;

    printf("entries: %d\n", entries);

    int i = 0;
    while(i < entries) {

      // seek to the i-th program header

      lseek(file, elf.phoff + i * sizeof(struct proghdr), SEEK_SET);

      read(file, &ph, sizeof(struct proghdr));
      if(ph.type == ELF_PROG_FLAG_EXEC ) {

        printf("ph.memsz:%x\n", ph.memsz); 
	src = mmap(NULL, ph.memsz, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE, 0, 0);

	//lseek(file, ph.off, SEEK_SET); 
        //read(file, src, ph.memsz); 

        printf("src: %p\n", src);
        printf("elf.entry: %x, ph.vaddr: %x\n", elf.entry, ph.vaddr);
	entry = src + (elf.entry - ph.vaddr); 

      }
      i++;
    }
    
    printf("entry:%p\n", entry); 

    if (entry != NULL) {
        sum = entry; 

	// for (....) 
        ret = sum(1, 2);
        printf("sum:%d\n", ret); 
    };


}
