#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define SIZE 1024*64
#define MAGIC 333

static void writem(int *mb, int fd, int magic){
  char *buf;
 
  buf = malloc(SIZE);
  if(!buf) {
    printf(2,"Couldn't allocate %d\n", SIZE);
    exit();
  }
    
  *(int*)buf = magic;

  int cc = write(fd, buf, SIZE);
  if(cc <= 0) {
    printf(1, "write failed %d\n", cc);
    return;
  }
    
  //printf(1,"%d ", (*mb)*SIZE);
  printf(1,".");
  (*mb)++;
    
  free(buf);
  return;
}


static void readm(int fd, int magic){
  char *buf;
  uint i = 0;
  int read_;

  buf = malloc(SIZE);
  if(!buf){
    printf(2,"Couldn't allocate 1MB\n");
    exit();
  }
    
  read_ = read(fd, buf, SIZE);
  //printf(1,"read %d bytes\n",read_);
  printf(1,".");
  if(read_ < 0){
    printf(1,"failed to read at iteration i %d\n",i);
  }

  if(*(int *)buf != magic) {
    printf(1,"bad data at iteration: %d expected value %d got %d\n", 
      i, magic, *(int*)buf);
  }
  
  free(buf);
  return;
}


int
main()
{
  int i, j;
  int mb = 1;
  int fd;
  char *names[] = {"file1", "file2", "file3", "file4"};
  int fsize[] = {1*16, 2*16, 4*16, 8*16};

  for(i = 0; i < 4; i++){
    fd = open(names[i], O_CREATE | O_WRONLY);
    if(fd < 0){
      printf(2, "big: cannot open file for writing\n");
      exit();
    }
    printf(1,"\nwriting file %s\n", names[i]);
    for(j = 0; j < fsize[i]; j++) {
      writem(&mb, fd, MAGIC*j);
    }

    close(fd);

    mb=1;
  }

  for(i = 0; i < 4; i++){
    fd = open(names[i],O_RDONLY);
    if(fd < 0){
      printf(2,"big: cannot open file for reading %s\n", names[i]);
      exit();
    }

    printf(1,"\nreading file %s\n", names[i]);
    for(j = 0; j < fsize[i]; j++){
      readm(fd, MAGIC*j);
    }
    close(fd);
  }

  exit();
}
