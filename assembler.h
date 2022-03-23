#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "constants.h"
#include "instructions.h"

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
    InstructionData *instructionData;
    int *dataImage;
    int IC;
    int DC;
} IterationsData;

IterationsData *startAssemblerFirstIteration(char *fileName);
int *startSecondIteration(IterationsData *firstIterData);
void createOutputFiles(char *origFileName, int *binaryInstructions, IterationsData *assemblerIterationsData);

#endif