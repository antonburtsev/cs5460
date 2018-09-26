#include <stdio.h>

void func_a(void){
    printf("func_a\n");
    return;
}

void func_b(void){
    printf("func_b\n");
    return;
}

int main(int ac, char **av)
{
    void (*fp)(void); 

    fp = func_b;
    fp();
    return;
}
