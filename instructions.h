#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "constants.h"

typedef struct {
    uint8_t addressingMethod;
    short numOfWords;
    char label[MAX_SYMBOL_NAME_LENGTH];
    union {
        int registerNum;
        short value;
    };
} operandData;

/* typedef struct instructionData *InstructionDataPtr; */

typedef struct instruction_data {
    char command[32];
    int lineNum;
    int IC;
    int words;
    operandData *sourceOperand;
    operandData *destinationOperand;
    struct instruction_data *next;
} InstructionData;


int saveInstructionData(char *command, int ic, int words, operandData *sourceOperand, operandData *destinatinoOperand, InstructionData **instructionsHead, int lineNum);

#endif
