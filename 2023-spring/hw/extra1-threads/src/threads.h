#if !defined(_THREADS_H_)
#define _THREADS_H_

#define EMPTY     0
#define RUNNABLE  1
#define EXITED    2

struct thread {
  void  *ebp;
  void  *esp;
  int   state;
  void *stack; 
  int   id; 
};


int u_thread_create(void *fnc, void*arg);

void u_yield();
void u_yield_to(struct thread *t);

int u_thread_join(); 
void u_sched();

#endif 


