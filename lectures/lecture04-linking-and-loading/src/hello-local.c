#include <stdio.h>

char hello[] = "Hello"; 
int main(int ac, char **av)
{
    char world[] = "world!";
    printf("%s %s\n", hello, world);
    return 0;
}
