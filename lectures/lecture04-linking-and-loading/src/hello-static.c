#include <stdio.h>

char hello[] = "Hello"; 
int main(int ac, char **av)
{
    static char world[] = "world!";
    printf("%s %s\n", hello, world);
    return 0;
}
