#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isIndexAdressing(char operand[], char label[], int *registerNumber)
{
    char *insideBrackets;
    if ((insideBrackets = strrchr(operand, '[')) != NULL)
    {
        printf("strings difference is: %d\n", strlen(operand) - strlen(insideBrackets));
        strncpy(label, operand, strlen(operand) - strlen(insideBrackets));
        printf("got into if statement: %s\n", label);
        if (strlen(insideBrackets) == 5 && insideBrackets[1] == 'r')
        {
            char registerNumberChar[10];
            *registerNumber = atoi(strncpy(registerNumberChar, (insideBrackets + 2), 2));
            if (*registerNumber <= 15 && *registerNumber >= 10) return 2;
            printf("Register number is not supported - only a register between 10 to 15 is allowed but got %d\n", *registerNumber);
        }
        printf("Illegal direct index register addressing for operand: %s\n", operand);
    }
    return 0;
}

int main(int argc, char const *argv[])
{
    // char test[30] = "mov    x[r12]     ";
    // int res = computeCommandWords(test, FALSE, "mov"); // isAllowedAddressingMethodByCommand("jsr", DIRECT_ADDRESING, SOURCE);
    // printf("result is: %d\n", res);
    // if (argc >= 2)
    // {
    //     printf("got %s source file", argv[1]);
    //     startAssemblerFirstIteration(argv[1]);
    // }

    char label[20];
    memset(label, '\0', sizeof(label));
    int registerNum;
    int result = isIndexAdressing("rds[r12]", &label, &registerNum);
    printf("result is: %d, lable is: %s, register is: %d\n", result, label, registerNum);
    printf("label length is: %d", strlen(label));
    return 0;
}

// int main(int argc, char const *argv[])
// {
//     char one[16], two[10];
//     char line[10] = "sakldjflkads";
//     sscanf(line, "%s%s", one, two);
//     printf("one is: %s, two is: %d", one, *two);
//     if (*two == NULL) printf("two is null");
//     return 0;
// }
