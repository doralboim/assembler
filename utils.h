#ifndef HANDLE_SYMBOL_TABLE_H
#define HANDLE_SYMBOL_TABLE_H

#include "assembler.h"
int *insertSymbolToTable(SymbolNode *table, char *symbol, int IC, int attribute);
int isLegalSymbol(char *symbol);
int isRegister(char *symbol);
int isInstruction(char *symbol);
SymbolNode *createNewSymbol(char *symbolName, int IC, SymbolAttribute attribute);

#endif