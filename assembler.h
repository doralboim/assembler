#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "constants.h"

typedef struct instructionData *InstructionDataPtr;

typedef struct instructionData {
    int byteCode;
    int InstructionWords;
    int IC;
    InstructionDataPtr *next;
} InstructionData;

typedef enum {
    DATA, ENTRY, EXTERNAL, CODE, EMPY
} SymbolAttribute;

typedef struct symbol_node {
    char symbolName[MAX_SYMBOL_NAME_LENGTH];
    int value;
    SymbolAttribute attribute1;
    SymbolAttribute attribute2;
    struct symbol_node *next;
} SymbolNode;


typedef struct {
    SymbolNode *symbolTable;
    int *instructionImage;
    int *dataImage;
} IterationsData;

IterationsData *startAssemblerFirstIteration(char *fileName);

#endif