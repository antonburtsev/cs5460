#include <stdio.h>

int y;

static int inc(int a) {
   return a + 1;
}

int dec(int b) {
   return inc(b) - 1;
}

void main () {
   int x;

   x = 5; 
   y = dec(x); 

   printf("result:%d\n", y); 
}
