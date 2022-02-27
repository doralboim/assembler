#ifndef UTIL_H
#define UITL_H

#include "assembler.h"
int *insertSymbolToTable(SymbolNode *table , InstructionNode **instructions, char *symbol, int IC, int attribute);
int isLegalSymbol(char *symbol, InstructionNode **instructions);
int isRegister(char *symbol);
int isInstruction(char *symbol, InstructionNode **instructions);
SymbolNode *createNewSymbol(char *symbolName, int IC, SymbolAttribute attribute);

#endif