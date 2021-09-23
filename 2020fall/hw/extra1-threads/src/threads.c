#include <stdio.h>
#include <stdlib.h>
#include <x86intrin.h>
#include "threads.h"

struct thread parent; 
struct thread *current; 

#define MAX_THREADS 16
struct thread threads[MAX_THREADS];

void _uswitch(void *from, void *to) __attribute__((returns_twice));

__asm__ ("      .text                     \n\t"
         "      .align  16                \n\t"
         "      .globl  _uswitch          \n\t"
         // 8(%esp): thread_to, 4(%esp): thread_from
         "_uswitch:                       \n\t"
         "      mov 4(%esp), %eax         \n\t" // load thread_from into eax
         "      mov 8(%esp), %ecx         \n\t" // load thread_to into ecx
         "      push %ebx                 \n\t" // save callee saved registers: ebx, edi, esi 
         "      push %edi                 \n\t" 
         "      push %esi                 \n\t"
         "      movl %ebp, 0(%eax)        \n\t" // save EBP
         "      movl %esp, 4(%eax)        \n\t" // save ESP
         // Thread state is saved, switch
         "      mov 0(%ecx), %ebp         \n\t" // load thread_to's ebp into ebp
         "      mov 4(%ecx), %esp         \n\t" // load thread_to's esp into esp
         "      pop %esi                  \n\t" // restore callee saved registers
         "      pop %edi                  \n\t" 
         "      pop %ebx                  \n\t"
         "      ret                       \n\t" // return 
        );

void u_thread_exit(void) {
  current->state = EXITED;
  _uswitch(current, &parent);
}

struct thread *alloc_thread() {
    for(int i = 0; i < MAX_THREADS; i++)
        if (threads[i].state == EMPTY) 
            return &threads[i]; 
    return NULL;
};

void free_thread(struct thread *t) {
    t->state = EMPTY;
};



int u_thread_create(void *fnc, void*arg) {

    struct thread *t = alloc_thread();
    if (t == NULL) 
	    return -1;
    
    current = t;    

    t->stack = malloc(4096); 
    if (t->stack == NULL) {
	    free_thread(t); 
	    return -1;
    }

    t->state = RUNNABLE; 
    t->esp = t->stack + 4096;

    // push the argument on the stack
    t->esp -= sizeof(void*);
    *(void**)t->esp = arg; 

    // when fnc returns, return into uexit() 
    t->esp -= sizeof(void*);
    *(void**)t->esp = u_thread_exit; 

    // The new thread will return into fnc from _uswitch
     t->esp -= sizeof(void*);
    *(void**)t->esp = fnc;
 
    // Fake the return stack for _uswitch_save 
    t->esp -= sizeof(void*);
    *(void**)t->esp = 0;     // ebx
    
    t->esp -= sizeof(void*);
    *(void**)t->esp = 0;     // edi

    t->esp -= sizeof(void*);
    *(void**)t->esp = 0;     // esi

    _uswitch(&parent, t);
    return t->id;
} 

void u_yield() {
    _uswitch(current, &parent);
} 

void u_yield_to(struct thread *t) {
    current = t;
    _uswitch(&parent, t);
} 

int u_thread_join() {
    return 0;
};

void u_sched() {

    // Example for how to track time (in CPU cycles)
    unsigned long long tsc =  __rdtsc(); 
    //printf("tsc:%llu\n", tsc); 
    
    // Dummy schedule
    while(threads[0].state == RUNNABLE || threads[1].state == RUNNABLE) {
        if (threads[0].state == RUNNABLE)
            u_yield_to(&threads[0]); 
        if (threads[1].state == RUNNABLE)
            u_yield_to(&threads[1]);
    }

};

