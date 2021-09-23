#include <stdio.h>
#include <stdlib.h>
#include "threads.h"

void do_work(void *arg) {
   int i; 
   for (i = 0; i < 2; i++) {
       printf("I'm in %s\n", (char *)arg);
       u_yield();
   }
};

int main(int argc, char *argv[]) {
    char a1[] = "Thread 1";
    char a2[] = "Thread 2";

    u_thread_create(do_work, (void*)a1);
    u_thread_create(do_work, (void*)a2); 

    u_sched();
    printf("Threads finished\n");
    return 0;
}



