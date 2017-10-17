#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"


static void write16MB(int *mb, int fd){
  char *buf;
  int  sectors;
  int count=0;
  buf = malloc(1048576);
  if(!buf){
    printf(2,"Couldn't allocate 1MB\n");
    exit();
  }
  sectors = 0;
  while(1){
  retry:
    *(int*)buf = sectors;
    int cc = write(fd, buf, 1048576);
    if(cc <= 0){
      printf(1,"write was %d\n",cc);
      count++;
      if(count==5){
	break;
      }
      goto retry;
    }
    sectors+=2048;
    printf(1,"%dMB ",*mb);
    (*mb)++;
    
    if(sectors == 32768){
      break;
    }
  }
  free(buf);
}


static void read16MB(int fd){
  char *buf;
  uint i = 0;
  int read_;
  buf = malloc(1048576);
  if(!buf){
    printf(2,"Couldn't allocate 1MB\n");
    exit();
  }
  while(i<16){
    read_ = read(fd,buf,1048576);
    printf(1,"read is %d\n",read_);
      if(read_ < 0){
	printf(1,"failed to read at iteration i %d\n",i);
      }
    if(*(int *)buf != (2048*i)){
      printf(1,"bad data at iteration i %d expected %d got %d\n",i,2048*i,*(int*)buf);
    }
    i++;
  }
}






int
main()
{
  int i,j;
  int mb=1;
  int fd;
  char *names[] = {"file1","file2","file3","file4"};
  for(i=0;i<4;i++){
    fd = open(names[i], O_CREATE | O_WRONLY);
    if(fd < 0){
      printf(2, "big: cannot open file for writing\n");
      exit();
    }
    printf(1,"\nwriting file %s\n",names[i]);
    for(j=(4-i);j<=4;j++){
      write16MB(&mb,fd);
    }
    close(fd);
    mb=1;
  }

  for(i=0;i<4;i++){
    fd = open(names[i],O_RDONLY);
    if(fd<0){
      printf(2,"big: cannot open file for reading %s\n",names[i]);
      exit();
    }
    for(j=(4-i);j<=4;j++){
      read16MB(fd);
    }
    close(fd);
  }

  exit();
}
