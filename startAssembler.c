#include <stdio.h>
#include "preprocessor.h"


int main(int argc, char *argv[])
{
    // int *preprocessedFile;

    while (--argc > 0)
    {
        /* preprocessedFile = processFile(*++argv); */
        printf("%s\n", *++argv);
    }

    return 1;   
}