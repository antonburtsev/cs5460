#include<stdio.h>

int a = 3;

static int add (int a, int b) {
    return a+b; // 
}

int main() {
    int b;

    b = 4;  // no - b is a local variable allocated on the stack

    int ret = add(a, b); // relative invocation (a is static -- has to be relocated) 

    printf("Result: %u\n", ret); // printf is relocated (it's not even known) 000000 
    				 // "Result: %u\n" -- R/O data so has to be relocated 
    return 0;
}
