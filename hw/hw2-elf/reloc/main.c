#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <sys/wait.h>

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

struct sectionhr {
  unsigned int name;
  unsigned int type;
  unsigned int shflags;
  unsigned int shaddr;
  unsigned int shoff;
  unsigned int size;
  unsigned int shlink;
  unsigned int shinfo;
  unsigned int align;
  unsigned int sh_entize;
};

struct Elf32_Rel{
	unsigned int r_offset;
	unsigned int r_info;
};

// Values for Proghdr type
#define ELF_PROG_LOAD           1

// Flag bits for Proghdr flags
#define ELF_PROG_FLAG_EXEC      1
#define ELF_PROG_FLAG_WRITE     2
#define ELF_PROG_FLAG_READ      4

#define E_IDNET_LEN 6
const char* MAGIC ="\x7f""ELF";

static const int DEBUG = 0;

void my_printf(const char *fmt, ...) {
    if (DEBUG) {
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
    }
}


int main(int argc, char* argv[]) {
    struct elfhdr elf;
    struct proghdr ph;
    int (*sum)(int a, int b);
    void *entry = NULL;
    int ret; 
    if (argc == 1) {
        my_printf("Usage: elf file name\n");
        exit(1);
    }

    FILE* fptr = fopen(argv[1], "r");
    if (fptr == NULL) {
        my_printf("Error loading %s", argv[0]);
        exit(1);
    }

    int bytes;
    if((bytes = fread(&elf, sizeof(struct elfhdr) , 1, fptr)) <= 0) {
        my_printf("Error loading elfhd %d\n", bytes);
        exit(1);
    }
    if(fread(&ph, sizeof(struct proghdr ) , 1, fptr) <= 0) {
        my_printf("Error loading proghdr\n");
        exit(1);
    }
    my_printf("Mem size: %d; offset: %d\n", ph.memsz, ph.off);
    void* code_va = mmap(NULL, ph.memsz, PROT_READ | PROT_WRITE | PROT_EXEC,
              MAP_ANONYMOUS | MAP_PRIVATE, 0, 0);
    fseek(fptr, ph.off, SEEK_SET);
    if(fread(code_va, ph.memsz, 1, fptr) <= 0) {
        my_printf("Error reading mem");
        exit(1);
    }
    char* test = (char*) code_va;
    my_printf("entry: %d;va: %zu; first: %c\n", elf.entry, (size_t) code_va, test[0]);
    my_printf("loading sectionhr\n");


    my_printf("ph num: %d; Elf shnum: %d\n", elf.phnum, elf.shnum);
    struct sectionhr** shrs = (struct sectionhr**) malloc(elf.shnum * sizeof(struct sectionhr*));
    int k = 0;
    for(int i = 0; i < elf.shnum; i++) {
        fseek(fptr, i * elf.shentsize + elf.shoff, SEEK_SET);
        struct sectionhr* shr = (struct sectionhr*) malloc(sizeof(struct sectionhr));
        if(fread(shr, sizeof(struct sectionhr), 1, fptr)<= 0) {
            my_printf("Error loading sectionhr");
            exit(1);
        }
        shrs[k++] = shr;
    }
    
    struct sectionhr* secname = shrs[elf.shstrndx];
    my_printf("Secname: %d\n", secname->size);
    char* nametable = (char*) malloc(sizeof(char) * secname->size);
    fseek(fptr, secname->shoff, SEEK_SET);
    fread(nametable, secname->size, 1, fptr);


    for(int i = 0; i < elf.shnum; i++) {
        struct sectionhr* shr = shrs[i];
        // Find the relocation section header, get the ".rel.text" section and use the info in Elf32_Rels to fix the address 
    }

    entry = (int (*)(int, int))code_va;

    fclose(fptr);
    if (entry != NULL) {
        sum = entry; 
        ret = sum(1, 2);
        printf("sum:%d\n", ret); 
    };

    
}


