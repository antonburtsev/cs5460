#include<stdio.h>

int a = 3;

static int add (int a, int b) {
    return a+b;
}

int main() {
    int b;

    b = 4;

    int ret = add(a, b);

    printf("Result: %u\n", ret);
    return 0;
}
