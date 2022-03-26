#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "constants.h"
#include "instructions.h"

typedef enum {
    EMPTY, DATA, ENTRY, EXTERNAL, CODE
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
    long *dataImage;
    int IC;
    int DC;
} IterationsData;

IterationsData *startAssemblerFirstIteration(char *fileName);
long *startSecondIteration(IterationsData *firstIterData);
void createOutputFiles(char *origFileName, long *binaryInstructions, IterationsData *assemblerIterationsData);

#endif
