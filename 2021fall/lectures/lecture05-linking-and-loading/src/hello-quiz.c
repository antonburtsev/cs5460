#include<stdio.h>
int a = 8;
int b = 5;

static int add (int a, int b) {
    return a + b;
}

int main() {
   int c;

   c = 4;
   c += a;
   c += b;
   int ret = add(c, 14);

   printf("Result: %u\n", ret);

   return ret;
}

