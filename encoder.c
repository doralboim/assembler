#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"
#include "assembler.h"

int getCommandFunct(char *command);
getAddressingNum(uint8_t addressingMehtod);
int registerByOperand(operandData *operand);

unsigned int generate_a_r_e(char A_R_E_Value)
{
    switch (A_R_E_Value)
    {
    case 'a':
        return 1 << 18;
    case 'r':
        return 1 << 17;
    case 'e':
        return 1 << 16;
    default:
        printf("Invalid A/R/E request, allowed values are a / r / e but got %c", A_R_E_Value);
        return 0;
    }
}


int encodeFirstWord(char *command)
{
    unsigned int optcode, a_r_e, i;
    CommandNode commands[] = COMMANDS;
    a_r_e = generate_a_r_e('a');
    for (i = 0; i < COMMANDS_AMOUNT; i++)
    {
        if (strcmp(commands[i].name, command) == 0)
        {
            printf("opcode is %d\n", commands[i].opcode);
            optcode = (1 << commands[i].opcode);
            break;
        }
    }
    
    return optcode | a_r_e;
}


int encodeSecWord(char *command, operandData *sourceOperand, operandData *destinationOperand)
{
    printf("start encoding second word for %s\n", command);
    int sourceEncoding = 0, desEncoding = 0;
    int funct = getCommandFunct(command);
    int sourceAddressing = sourceOperand != NULL? getAddressingNum(sourceOperand->addressingMethod) : 0;
    int destinationAddressing = getAddressingNum(destinationOperand->addressingMethod);

    printf("got registers\n");
    funct = funct << 12;
    sourceAddressing = sourceAddressing << 6;
    
    int sourceRegister = sourceOperand != NULL? registerByOperand(sourceOperand) : 0;
    int destinationRegister = registerByOperand(destinationOperand);
    sourceRegister = sourceRegister << 8;
    destinationRegister = destinationRegister << 2;
    // if (sourceRegister != -1) sourceEncoding = encodeDirectAdressing(sourceRegister);
    // if (desRegister != -1) desEncoding = encodeDirectAdressing(desRegister);
    
    unsigned a_r_e = generate_a_r_e('a');
    printf("returning second word\n");
    return a_r_e | funct | sourceRegister | sourceAddressing | destinationRegister | destinationAddressing;
}


int encodeRegisterDirectAdressing(int registerNum, OperandType type)
{
    return type == DESTINATION? (registerNum << 2) | 3 : (registerNum << 8) | (3 << 6);
}


int registerByOperand(operandData *operand)
{
    printf("getting register by operand\n");
    if (operand->addressingMethod == IMMEDIATE_ADDRESING || operand->registerNum == -1) return 0;
    return operand->registerNum;
}


int *encodeDirectAdressing(SymbolNode *label)
{
    printf("encoding direct addressing for %s!\n", label);
    int *result = (int *) malloc(2 * sizeof(int));
    
    if (label->attribute1 != EXTERNAL)
    {
        printf("label is not external\n");
        *result = generate_a_r_e('r') | (label->value) / 16;
        *(result + 1) = generate_a_r_e('r') | (label->value) % 16;
        printf("result is defined\n");
    }
    
    else
    {
        printf("label is external\n");
        *result = generate_a_r_e('e');
        *(result + 1) = generate_a_r_e('e');
    }

    printf("returning result!\n");
    return result;
}

int encodeImmediateAdressing(int operandValue)
{
    return generate_a_r_e('a') | operandValue;
}

/* return the command funct code */
int getCommandFunct(char *command)
{
    CommandNode ptr[] = COMMANDS;
    int i;
    for (i=0; i < COMMANDS_AMOUNT && strcmp(ptr[i].name, command) != 0; i++) ;
    return ptr[i].funct;
}

/* return the decimal number of a given bitfield addressing method */
getAddressingNum(uint8_t addressingMehtod)
{
    const AddressMethodWords addressingMethods[] = ADDRESSING_METHODS;
    int i;
    for (i = 0; i != NULL && addressingMethods[i].method != addressingMehtod != 0; i++) ;
    return addressingMethods[i].method;
}
