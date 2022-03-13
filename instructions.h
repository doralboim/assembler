#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "constants.h"

typedef struct {
    uint8_t addressingMethod;
    char label[MAX_SYMBOL_NAME_LENGTH];
    union {
        int registerNum;
        int value;
    };
} operandData;

typedef struct instructionData *InstructionDataPtr;

typedef struct instruction_data {
    char command[32];
    int IC;
    int words;
    operandData *sourceOperand;
    operandData *destinationOperand;
    InstructionDataPtr *next;
} InstructionData;


void saveInstructionData(char *command, int ic, int words, operandData *sourceOperand, operandData *destinatinoOperand, InstructionData *instructionsDataHead);

#endif