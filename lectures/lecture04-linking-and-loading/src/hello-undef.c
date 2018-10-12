#include<stdio.h>
extern int add (int a, int b); 

int main() {
    int a,b;
    a = 3;
    b = 4;

    int ret = add(a,b);

    printf("Result: %u\n", ret);
    return 0;
}
