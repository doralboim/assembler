#ifndef HANDLE_SYMBOL_TABLE_H
#define HANDLE_SYMBOL_TABLE_H

#include <stdlib.h>
#include "assembler.h"
#include "instructions.h"

/* from utils */
int insertSymbolToTable(SymbolNode *tableHead, char *symbolName, int IC, SymbolAttribute attribute);
int isLegalSymbol(char *symbol);
int isRegister(char *symbol);
int isCommand(char *symbol);
SymbolNode *createNewSymbol(char *symbolName, int IC, SymbolAttribute attribute);
int isValidCommand(char *action);
int getNumberOfAllowedOperandsByCommand(char *command);
int isAllowedAddressingMethodByCommand(char command[], uint8_t addressMethod, OperandType operandType);
int getWordsNumByAdressMethod(uint8_t addressMethod);
int getNumberOfCommas(char *line);
int isDirectAdressing(char operand[]);
operandData *addressingMethodByOperand(char operand[]);                         

#endif