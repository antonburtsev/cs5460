#include <stdio.h>


int main() {
  char *t = "dax";
  int a1 = (int) t;
  int a2 = (int) *t;
  int a = (int) *(int*)t;
  printf("t = %p | a1 = 0x%x\n", t, a1);
  printf("t = %p | a2 = 0x%x('%c')\n", t, a2, a2);
  printf("t = %p | a = 0x%x\n", t, a);
  return 0;
}
