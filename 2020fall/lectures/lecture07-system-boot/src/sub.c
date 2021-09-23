#include <stdio.h>

int main() {

    unsigned int a = 0;
    unsigned int b = 0xffffffff;
    unsigned int c = 0xfe000000;


    printf("0 - 0xffffffff:%u\n", (unsigned int) (a - b));
    printf("-1:%u\n", (unsigned int) (-1));
    printf("-0xffffffff:%u\n", (unsigned int) (-0xffffffff));
    printf("-0xfe000000:%u\n", (unsigned int) (-0xfe000000));
    printf("0 - 0xfe000000:%u\n", (unsigned int) (a - c));

    return;
}
