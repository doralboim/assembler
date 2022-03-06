#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"
#include "assembler.h"

unsigned int generate_are(char are_value)
{
    switch (are_value)
    {
    case 'a':
        return 1 << 18;
    case 'r':
        return 1 << 17;
    case 'e':
        return 1 << 16;
    default:
        printf("Invalid ARE request, allowed values are a / r / e but got %c", are_value);
        return 0;
    }
}

int encodeFirstWord(char *command)
{
    unsigned int optcode, are, i;
    are = generate_are('a');
    for (i = 0; i < COMMANDS_AMOUNT; i++)
    {
        if (strcmp(COMMANDS[i].name, command) == 0)
        {
            printf("opcode is %d\n", COMMANDS[i].opcode);
            optcode = (1 << COMMANDS[i].opcode);
            break;
        }
    }
    
    return optcode | are;
}

int encodeRegisterDirectAdressing(int registerNum, OperandType type)
{
    return type == DESTINATION? (registerNum << 2) | 3 : (registerNum << 8) | (3 << 6);
}

int *encodeDirectAdressing(SymbolNode *label)
{
    int result[2];
    if (strcmp(label->attribute1, "extern") != 0 && strcmp(label->attribute2, "extern") != 0)
    {
        result[0] = generate_are('r') | (label->value) / 16;
        result[1] = generate_are('r') | (label->value) % 16;
    }
    else
    {
        result[0] = generate_are('e');
        result[1] = generate_are('e');
    }

    return result;
}

int encodeSecWord(int sourceRegister, int desRegister)
{
    int sourceEncoding = 0, desEncoding = 0;
    if (sourceRegister != -1) sourceEncoding = encodeDirectAdressing(sourceRegister);
    if (desRegister != -1) desEncoding = encodeDirectAdressing(desRegister);
    
    return sourceEncoding | desEncoding;
}

int encodeImmediateAdressing(int operandValue)
{
    return generate_are('a') | operandValue;
}

int main(int argc, char const *argv[])
{
    char x[12] = "lea";
    int e = encodeImmediateAdressing(254);
    printf("result is %d", e);
    return 0;
}
