#include <stdio.h>


int main() {
  char *t = "dax";
  char t1[4] = {'d', 'a', 'x', '\0'};
  printf("t = %p | t1 = %p\n", t, t1);
  printf("Value at t = %s | t1 = %s\n", t, t1);
  return 0;
}
