#include <stdio.h>

extern int a(char *s);

int main(int ac, char **av)
{
    static char string[] = "Hello world\n";
    a(string);
    return 0;
}
