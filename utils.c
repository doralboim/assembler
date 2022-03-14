#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "assembler.h"
#include "constants.h"
#include "instructions.h"
#include "utils.h"

#define isInstruction(act) strcmp(act, ".data") || strcmp(act, ".string") || \
                           strcmp(act, ".entry") || strcmp(act, ".extern")

/* check if the given command is presented in the machine allowed commands */
int isValidCommand(char *command)
{
    CommandNode commands[] = COMMANDS;
    int i;
    for (i = 0; i < COMMANDS_AMOUNT; i++)
    {
        if (strcmp(command, commands[i].name)) return TRUE;
    }
    return FALSE;
}

/* check if a symbol is a register and return its number if so, otherwise return -1*/
int isRegister(char *symbol)
{
    char *ptr = (char *) malloc(strlen(symbol));
    char *charRegisterNumber;
    int registerNum;

    strcpy(ptr, symbol);
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
            if (registerNum < REGISTERS_AMOUNT) return registerNum;
        }
    }
    
    return -1;
}

/* return the number of allowed operands for a given command */
int getNumberOfAllowedOperandsByCommand(char *command)
{
    CommandNode commands[] = COMMANDS;
    int i, allowedSource, allowedDes;
    for (i = 0; i < COMMANDS_AMOUNT; i++)
    {
        if (strcmp(command,commands[i].name) == 0)
        {
            allowedSource =commands[i].sourceAdressingMethods == 0? 0: 1;
            allowedDes =commands[i].targetAdressingMethods == 0? 0: 1;
            return allowedDes + allowedSource;
        }
    }
    return -1;
}

/* check if a given addressing method is allowed for the given operand type (source / destination) with a given command */
int isAllowedAddressingMethodByCommand(char command[], uint8_t addressMethod, OperandType operandType)
{
    CommandNode commands[] = COMMANDS;
    printf("got commands for %s\n", command);
    if (operandType != SOURCE && operandType != DESTINATION)
    {
        printf("Invalid opernad type - expected source - %d or destination - %d but got %d\n", SOURCE, DESTINATION, operandType);
        return FALSE;
    }
    int i = 0;
    for(i = 0 ; i < COMMANDS_AMOUNT; i++)
    {
        if (strcmp(command, commands[i].name) == 0)
        {
            uint8_t opType = operandType == SOURCE? commands[i].sourceAdressingMethods: commands[i].targetAdressingMethods;
            if ((addressMethod & opType) > 0)
            {
                printf("allowed addresing method for %s\n", command);
                return TRUE;
            }
            printf("Invalid addressing method for command '%s'\n", command);
            return FALSE;
        }
    }
    printf("Invalid command: %s\n", command);
    return FALSE;
}

/* get the number of words for a given addressing method */
int getWordsNumByAdressMethod(uint8_t addressMethod)
{
    printf("getting words number for address method\n");
    const AddressMethodWords addressMethodWords[] = ADDRESSING_METHODS;
    printf("words per address method : %d %d %d\n", addressMethodWords->method, addressMethodWords->numOfWords, addressMethodWords->methodNum);
    int i;
    for (i = 0; i < ADDRESS_METHOD_AMOUNT; i++)
    {
        if (addressMethodWords[i].method == addressMethod)
        {
            printf("num of words for %d addresing method: %d\n", addressMethod, addressMethodWords[i].numOfWords);
            return addressMethodWords[i].numOfWords;
        }
    }
    printf("error computing number of words\n");
    return -1;
}

int getNumberOfCommas(char *line)
{
    int i;
    for (i=0; line[i]; line[i]==',' ? i++ : *line++);
    return i;
}

/* check if index addressing method and set lable and registerNumber accordingly, otherwise return FALSE */
int isIndexAdressing(char operand[], char label[], int *registerNumber)
{
    char *insideBrackets;
    if ((insideBrackets = strrchr(operand, '[')) != NULL)
    {
        strncpy(label, operand, strlen(operand) - strlen(insideBrackets));
        if (strlen(insideBrackets) == 5 && insideBrackets[1] == 'r')
        {
            char registerNumberChar[10];
            *registerNumber = atoi(strncpy(registerNumberChar, (insideBrackets + 2), 2));
            if (*registerNumber <= 15 && *registerNumber >= 10) return TRUE;
            printf("Register number is not supported - only a register between 10 to 15 is allowed but got %d\n", *registerNumber);
        }
        printf("Illegal direct index register addressing for operand: %s\n", operand);
    }
    return FALSE;
}

/* check if direct addressing method and update value accordingly, otherwise return FALSE */
int isImmediateAdressing(char operand[], long *value)
{
    if(operand[0] == '#')
    {
        char *end;
        *value = strtol(&operand[1], &end, 10);
        if (*end != '\0') return FALSE;
        return value;
    }
    return FALSE;
}

/* returns the given operand's addressing method and its according data - label & register * value */
operandData *addressingMethodByOperand(char operand[])
{
    int result;
    operandData *resultData = (operandData *) malloc(sizeof(operandData));

    int registerNum;
    char label[MAX_SYMBOL_NAME_LENGTH];
    memset(label, '\0', sizeof(label));

    long value;

    if((result = isRegister(operand)) != -1)
    {
        resultData->addressingMethod = REGISTER_DIRECT_ADDRESSING;
        resultData->registerNum = result;
        printf("register direct addressing\n");
    }
    
    else if((result =isIndexAdressing(operand, &label, &registerNum)))
    {
        resultData->addressingMethod = INDEX_ADDRESSING;
        strcpy(resultData->label, label);
        resultData->registerNum = registerNum;
        return resultData;
        printf("index addressing\n");
    }

    else if((result = isImmediateAdressing(operand, &value)))
    {
        resultData->addressingMethod = IMMEDIATE_ADDRESING;
        resultData->value = value;
        printf("immediate addressing\n");
    }

    else
    {   
        resultData->addressingMethod = DIRECT_ADDRESING;
        strcpy(resultData->label, operand);
        printf("direct addressing\n");
    }

    return resultData;
}

// int main(int argc, char const *argv[])
// {
//     operandData *test = addressingMethodByOperand("#5479");
//     printf("%d %s %d %d\n", test->addressingMethod, test->label, test->registerNum, test->value);
//     return 0;
// }
