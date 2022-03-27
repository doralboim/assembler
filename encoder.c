#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "constants.h"
#include "assembler.h"

int getCommandFunct(char *command);
int getAddressingNum(uint8_t addressingMehtod);
int registerByOperand(operandData *operand);

unsigned long generate_a_r_e(char A_R_E_Value)
{
    switch (A_R_E_Value)
    {
    case 'a':
        return (1 & 0xFFFF) << 18;
    case 'r':
        return (1 & 0xFFFF) << 17;
    case 'e':
        return (1 & 0xFFFF) << 16;
    default:
        return 0;
    }
}


long encodeFirstWord(char *command)
{
    unsigned long optcode, a_r_e, i;
    CommandNode commands[] = COMMANDS;
    a_r_e = generate_a_r_e('a');
    for (i = 0; i < COMMANDS_AMOUNT; i++)
    {
        if (strcmp(commands[i].name, command) == 0)
        {
            optcode = (1 << commands[i].opcode) & 0xFFFF;
            break;
        }
    }
    
    return optcode | a_r_e;
}


long encodeSecWord(char *command, operandData *sourceOperand, operandData *destinationOperand)
{
    long sourceEncoding = 0, desEncoding = 0;
    long funct = getCommandFunct(command);
    long sourceAddressing = sourceOperand != NULL? getAddressingNum(sourceOperand->addressingMethod) : 0;
    long destinationAddressing = getAddressingNum(destinationOperand->addressingMethod);

    funct = funct << 12;
    sourceAddressing = sourceAddressing << 6;
    
    long sourceRegister = sourceOperand != NULL? registerByOperand(sourceOperand) : 0;
    long destinationRegister = registerByOperand(destinationOperand);
    sourceRegister = sourceRegister << 8;
    destinationRegister = destinationRegister << 2;
    
    unsigned long a_r_e = generate_a_r_e('a');
    return a_r_e | funct | sourceRegister | sourceAddressing | destinationRegister | destinationAddressing;
}


long encodeRegisterDirectAdressing(int registerNum, OperandType type)
{
    return type == DESTINATION? ((registerNum << 2) | 3) & 0xFFFF : ((registerNum << 8) | (3 << 6)) & 0xFFFF;
}


int registerByOperand(operandData *operand)
{
    if (operand->addressingMethod == IMMEDIATE_ADDRESING || operand->numericValue.registerNum == -1) return 0;
    return operand->numericValue.registerNum;
}


/* Encode according to the label value (base + offset) */
long *encodeDirectAdressing(SymbolNode *label)
{
    long *result = (long *) malloc(2 * sizeof(long));
    
    if (label->attribute1 != EXTERNAL)
    {
        *result = generate_a_r_e('r') | (label->value) / 16;
        *(result + 1) = generate_a_r_e('r') | (label->value) % 16;
    }
    
    else
    {
        *result = generate_a_r_e('e');
        *(result + 1) = generate_a_r_e('e');
    }

    return result;
}

/* when operand is encoded using immediate addressing, A/R/E bit is A and the rest 16 bits are operand value */
long *encodeImmediateAdressing(short operandValue)
{
    long *result = (long *) malloc(sizeof(long));
    *result = (generate_a_r_e('a') | operandValue) & 0xFFFF;
    return result;
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
int getAddressingNum(uint8_t addressingMehtod)
{
    const AddressMethodWords addressingMethods[] = ADDRESSING_METHODS;
    int i;
    for (i = 0; i < ADDRESS_METHOD_AMOUNT && addressingMethods[i].method != addressingMehtod != 0; i++) ;
    return addressingMethods[i].method;
}

/* encode numnbers and characters and insert to the data image, returning the increased DC */
long encodeData(char line[], long dataImage[], int dc)
{
    char *instruction, *token, *stringData;
    int i = 1, countElements = 0, intToken;
    
    if ((instruction = strstr(line, DATA_INSTRUCTION)) != NULL)
    {
        token = strtok(instruction + strlen(DATA_INSTRUCTION), ",");
        while (token != NULL)
        {
            {
                sscanf(token, "%s", token);
                intToken = atoi(token);
                if (intToken > SHRT_MAX || intToken < SHRT_MIN)
                {
                    fprintf(stderr, "Data value exceeds exceptable range. Allowed range is %d - %d but got %d", SHRT_MIN, SHRT_MAX, intToken);
                    return -1;                    
                }
                dataImage[dc++] = generate_a_r_e('a') | ((short)intToken & 0xFFFF);
                countElements++;
                token = strtok(NULL, ",");
            }
        }
    }
    else
        {
            instruction = strstr(line, STRING_INSTRUCTION);
            stringData = strstr(line, "\"");
            while (*(stringData + i) != '"')
            {
                dataImage[dc++] = generate_a_r_e('a') | *(stringData + i++);
                countElements++;
            }
            // while ((dataImage[dc++] = generate_a_r_e("a") | *(stringData + i++)) != '"') countElements++;
            dataImage[dc - 1] = 0;
        }
    
    return countElements > 0 ? dc : -1;
}