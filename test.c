#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    char one[16], two[10];
    char line[10] = "sakldjflkads";
    sscanf(line, "%s%s", one, two);
    printf("one is: %s, two is: %d", one, *two);
    if (*two == NULL) printf("two is null");
    return 0;
}
