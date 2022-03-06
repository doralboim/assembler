#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "assembler.h"
#include "constants.h"
#include "utils.h"

#define isInstruction(act) strcmp(act, ".data") || strcmp(act, ".string") || \
                           strcmp(act, ".entry") || strcmp(act, ".extern")

int isValidAction(char *action)
{
    int i;
    for (i = 0; i < COMMANDS_AMOUNT; i++)
    {
        if (strcmp(action, COMMANDS[i].name)) return TRUE;
    }
    return FALSE;
}

int isRegister(char *symbol)
{
    char *ptr = (char *) malloc(strlen(symbol));
    char *charRegisterNumber;
    int registerNum;

    strcpy(ptr, symbol);
    printf("ptr is %s, first char is %c sec char is %c\n", ptr, *ptr, *(ptr + 1));
    if (strlen(ptr) == 2)
    {
        if (*ptr == 'r' && isdigit(*(ptr + 1)))
        {
            *charRegisterNumber = *(ptr + 1);
            return atoi(charRegisterNumber);
        }
    }
    else if (strlen(ptr) == 3)
    {
        if (*ptr == 'r' && isdigit(*(ptr + 1)) && isdigit(*(ptr + 2)))
        {
            strncpy(charRegisterNumber, ptr + 1, 2);
            registerNum = atoi(charRegisterNumber);
            if (char < REGISTERS_AMOUNT) return registerNum;
        }
    }
    
    return FALSE;
}

int getNumberOfAllowedOperandsByCommand(char *command)
{
    int i, allowedSource, allowedDes;
    for (i = 0; i < COMMANDS_AMOUNT; i++)
    {
        if (strcmp(command, COMMANDS[i].name) == 0)
        {
            allowedSource = COMMANDS[i].sourceAdressingMethods == 0? 0: 1;
            allowedDes = COMMANDS[i].targetAdressingMethods == 0? 0: 1;
            return allowedDes + allowedSource;
        }
    }
    return -1;
}

int isAllowedAddressingMethodByCommand(char command[], uint8_t addressMethod, OperandType operandType)
{
    if (operandType != SOURCE && operandType != DESTINATION)
    {
        printf("Invalid opernad type - expected source - %d or destination - %d but got %d\n", SOURCE, DESTINATION, operandType);
        return FALSE;
    }
    int i = 0;
    for(i = 0 ; i < COMMANDS_AMOUNT; i++)
    {
        if (strcmp(command, COMMANDS[i].name) == 0)
        {
            uint8_t opType = operandType == SOURCE? COMMANDS[i].sourceAdressingMethods: COMMANDS[i].targetAdressingMethods;
            if ((addressMethod & opType) > 0) return TRUE;
            printf("Invalid addressing method for command '%s'\n", command);
            return FALSE;
        }
    }
    printf("Invalid command: %s\n", command);
    return FALSE;
}

int getWordsNumByAdressMethod(uint8_t addressMethod)
{
    int i;
    for (i = 0; i < ADDRESS_METHOD_AMOUNT; i++)
    {
        if (addressMethodsWordsDict[i].method == addressMethod)
        {
            printf("num of words for %d addresing method: %d\n", addressMethod, addressMethodsWordsDict[i].numOfWords);
            return addressMethodsWordsDict[i].numOfWords;
        }
    }
    return -1;
}

int getNumberOfCommas(char *line)
{
    int i;
    for (i=0; line[i]; line[i]==',' ? i++ : *line++);
    return i;
}

/* check if index addressing method and return 2 if true, otherwise return 0 */
int isIndexAdressing(char operand[])
{
    char *insideBrackets;
    if ((insideBrackets = strrchr(operand, '[')) != NULL)
    {
        if (strlen(insideBrackets) == 5 && insideBrackets[1] == 'r')
        {
            char registerNumberChar[10];
            int *registerNumber = atoi(strncpy(registerNumberChar, (insideBrackets + 2), 2));
            if (registerNumber <= 15 && registerNumber >= 10) return 2;
            printf("Register number is not supported - only a register between 10 to 15 is allowed but got %d", registerNumber);
        }
        printf("Illegal direct index register addressing for operand: %s\n", operand);
    }
    return 0;
}

int isDirectAdressing(char operand[])
{
    if(operand[0] == '#')
    {
        char *end;
        long value = strtol(&operand[1], &end, 10);
        printf("vaue is %ld char is: %s\n", value, end);
        if (*end != '\0') return -1;
        return value;
    }
    return -1;
}

/* returns the given operand's addressing method */
int addressingMethodByOperand(char operand[])
{
    int result;

    if((result = isRegister(operand))) return result == -1 ? -1: REGISTER_DIRECT_ADDRESSING;    
    if((result =isIndexAdressing(operand))) return result == -1 ? -1: INDEX_ADDRESSING;
    if((result = isDirectAdressing(operand))) return result == -1 ? -1:  DIRECT_ADDRESING;

    return DIRECT_INDEX_REGISTER_ADDRESSING;
}