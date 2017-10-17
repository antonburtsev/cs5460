#include <stdio.h>

volatile int bar(int a, int b, int c) {
    printf("foo\n");
    return b + c;
}

int foo(int a, int b, int c, int d, int e, int f, int g, int h) {
    return a + c;
}

int main(void){

    return bar(1,2,3);
    //return foo(1, 2, 3, 4, 5, 6, 7, 8);
};
