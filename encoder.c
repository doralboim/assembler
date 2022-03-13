#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"
#include "assembler.h"

int getCommandFunct(char *command);
getAddressingNum(uint8_t addressingMehtod);

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
    CommandNode *commands = machine_commands();
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

int encodeSecWord(char *command, int sourceRegister, uint8_t sourceAddressingMehtod, int destinationRegister, uint8_t destinationAddressingMehtod)
{
    int sourceEncoding = 0, desEncoding = 0;
    int funct = getCommandFunct(command);
    int sourceAddressing = getAddressingNum(sourceAddressingMehtod);
    int destinationAddressing = getAddressingNum(destinationAddressingMehtod);

    funct = funct << 12;
    sourceAddressing = sourceAddressing << 6;
    
    sourceRegister = sourceRegister << 8;
    destinationRegister = destinationRegister << 2;
    // if (sourceRegister != -1) sourceEncoding = encodeDirectAdressing(sourceRegister);
    // if (desRegister != -1) desEncoding = encodeDirectAdressing(desRegister);
    
    unsigned a_r_e = generate_a_r_e('a');

    return a_r_e | funct | sourceRegister | sourceAddressing | destinationRegister | destinationAddressing;
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
        result[0] = generate_a_r_e('r') | (label->value) / 16;
        result[1] = generate_a_r_e('r') | (label->value) % 16;
    }
    else
    {
        result[0] = generate_a_r_e('e');
        result[1] = generate_a_r_e('e');
    }

    return result;
}

int encodeImmediateAdressing(int operandValue)
{
    return generate_a_r_e('a') | operandValue;
}

int main(int argc, char const *argv[])
{
    char x[12] = "lea";
    int e = encodeImmediateAdressing(254);
    printf("result is %d", e);
    return 0;
}

/* return the command funct code */
int getCommandFunct(char *command)
{
    CommandNode *ptr = machine_commands();
    int i;
    for (i=0; i < COMMANDS_AMOUNT && strcmp(ptr[i].name, command) != 0; i++) ;
    return ptr[i].funct;
}

/* return the decimal number of a given bitfield addressing method */
getAddressingNum(uint8_t addressingMehtod)
{
    const AddressMethodWords *addressingMethods = wordsPerAddressMethods();
    int i;
    for (i = 0; i != NULL && addressingMethods[i].method != addressingMehtod != 0; i++) ;
    return addressingMethods[i].method;
}