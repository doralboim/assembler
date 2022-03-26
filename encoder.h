#ifndef ENCODER_H
#define ENCODER_H

#include "assembler.h"

long encodeData(char line[], long dataImage[], int dc);
unsigned long generate_a_r_e(char A_R_E_Value);
long encodeFirstWord(char *command);
long encodeRegisterDirectAdressing(int registerNum, OperandType type);
long *encodeDirectAdressing(SymbolNode *label);
long encodeSecWord(char *command, operandData *sourceOperand, operandData *destinationOperand);
long *encodeImmediateAdressing(int operandValue);

#endif
