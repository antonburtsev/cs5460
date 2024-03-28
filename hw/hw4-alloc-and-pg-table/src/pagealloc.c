#include "mmu.h" // optional

#define MAX_PAGES (1 << 20) 
#define NIL 0xFFFFFFFF

struct page {
    unsigned int next;
};

struct free_pages {
    unsigned int head;
    struct page pages[MAX_PAGES];
};

struct free_pages page_allocator = {
  .head = NIL,
};

unsigned int page_alloc() 
{
  //TODO
}

void page_free(unsigned int pa) 
{
  //TODO
}

void mmap(pde_t* pgdir, unsigned int va, unsigned int pa, unsigned int flag) 
{
  //TODO
}

