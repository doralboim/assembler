#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "constants.h"
#include "utils.h"

#define isInstruction(act) strcmp(act, ".data") || strcmp(act, ".string") || \
                           strcmp(act, ".entry") || strcmp(act, ".extern")

int computeCommandWords(char line[], int isSymbol, char *action)
{
    int words = 1, commas;
    commas = getNumberOfCommas(line);
    if (commas > 1)
    {
        printf("Illegal number of operands, expected max of 2 and got %d", commas - 1);
        return 0;
    }
    if (commas == 1)
    {
        char *firstOperand, *secOperand;
        uint8_t firstAdressMethod, secAdressMethod;
        firstOperand = strtok(line, ",");
        secOperand = strtok(NULL, ",");
        if 
    }
    
}


int addressingMethodByOperand(char operand[])
{
    if(isRegister(operand))
        return 1;
    if(isIndex(operand))
        return 2;
}

int isIndex(char operand[])
{
    char *insideBrackets;
    if ((insideBrackets = strrchr(operand, "[")) != NULL)
    {
        if (strcmp(*(insideBrackets + 1), 'r') && isdigit(*(insideBrackets + 2)) && isdigit(*(insideBrackets + 3)) && strcmp(*(insideBrackets + 4), ']'))
        {
            char *registerNumber;
            if (atoi(strncpy(registerNumber, *(insideBrackets + 2), 2)) <= 15 &&
                atoi(strncpy(registerNumber, *(insideBrackets + 2), 2)) >= 10) return 2;
        }
    }
    return 0;
}

int commandWordsByAddressingMethod(uint8_t addressingMethod)
{
    switch (addressingMethod)
    {
    case IMMEDIATE_ADDRESING:
        return 1;
    case DIRECT_ADDRESING:
        return 2;
    case INDEX_ADDRESSING:
        return 3;
    case REGISTER_DIRECT_ADDRESSING:
        return 4;
    } addressingMethod;
}

int isLegalAction(char *action)
{
    return;
}