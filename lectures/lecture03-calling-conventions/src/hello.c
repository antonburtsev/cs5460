//#include <stdio.h>
#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"
#include "traps.h"
#include "memlayout.h"

main() {
  char * message = "aaa\n";
  int pid = fork();
  if(pid != 0){
    char *echoargv[] = { "echo", "Hello", 0 };
    // parent process
    message = "bbb\n";
    exec("echo", echoargv);
  }

  write(1, message, 4);
  exit();
}


