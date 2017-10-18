#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char hello[] = "Hello"; 
int main(int ac, char **av)
{
    char world[] = "world!";
    char *str = malloc(64); 
    memcpy(str, "beautiful", 64);
    printf("%s %s %s\n", hello, str, world);
    return 0;
}
