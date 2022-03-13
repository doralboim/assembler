#ifndef ENCODER_H
#define ENCODER_H

#include "assembler.h"

unsigned int generate_a_r_e(char A_R_E_Value);
int encodeFirstWord(char *command);
int encodeRegisterDirectAdressing(int registerNum, OperandType type);
int *encodeDirectAdressing(SymbolNode *label);
int encodeSecWord(char *command, int sourceRegister, uint8_t sourceAddressingMehtod, int destinationRegister, uint8_t destinationAddressingMehtod);
int encodeImmediateAdressing(int operandValue);

#endif