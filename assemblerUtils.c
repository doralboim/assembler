#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "assembler.h"
#include "constants.h"
#include "instructions.h"

SymbolNode *createNewSymbol(char *symbolName, int IC, SymbolAttribute attribute);
int isLegalSymbol(char *symbol, InstructionNode **instructions);
int isInstruction(char *symbol, InstructionNode **instructions);
int isRegister(char *symbol);


int *insertSymbolToTable(SymbolNode *tableHead , InstructionNode **instructions, char *symbolName, int IC, SymbolAttribute attribute)
{
    SymbolNode *ptr, *newSymbol;
    int i;
    
    if (isLegalSymbol(symbolName, instructions)) return FALSE;
    newSymbol = createNewSymbol(symbolName, IC, attribute);
    if (tableHead->value == 0)
    {
        printf("creating new symbol: %s - %d\n", newSymbol->symbolName, newSymbol->value);
        tableHead = newSymbol;
        return TRUE;
    }

    ptr = tableHead;
    while (ptr->symbolName != NULL)
    {
        printf("in the loop");
        if (strcmp(ptr->symbolName, symbolName) == 0)
        {
            printf("Symbol %s is already exist in the symbolName table!\n", symbolName);
            return FALSE;
        }
        ptr = ptr->next;
    }

    ptr->next = newSymbol;
    
    return TRUE;
}


SymbolNode *createNewSymbol(char *symbolName, int IC, SymbolAttribute attribute)
{
    SymbolNode *newSymbol;

    newSymbol = (SymbolNode *) malloc(sizeof(SymbolNode));

    strcpy(newSymbol->symbolName, symbolName);
    newSymbol->value = IC;
    newSymbol->attribute1 = attribute;

    return newSymbol;
}

int isLegalSymbol(char *symbol, InstructionNode **instructions)
{
    return (isRegister(symbol) || isInstruction(symbol, instructions));
}

int isRegister(char *symbol)
{
    char *ptr = (char *) malloc(strlen(symbol));
    strcpy(ptr, symbol);
    
    if (strlen(ptr) == 2)
    {
        if (*ptr == 'r' && isdigit(*(ptr + 1)))
        {
            printf("symbol %s is a register!\n", symbol);
            return TRUE;
        }
    }
    else if (strlen(ptr) == 3)
    {
        if (*ptr == 'r' && isdigit(*(ptr + 1)) && isdigit(*(ptr + 2)))
        {
            printf("symbol %s is a register!\n", symbol);
            return TRUE;
        }
    }
    
    return FALSE;
}

int isInstruction(char *symbol, InstructionNode **instructions)
{
    int i;
    for (i = 0; i < INSTRUCTIONS_AMOUNT; i++)
    {
        if (strcmp(symbol, instructions[i]->name) == 0)
        {
            printf("symbol %s is an instruction: %s!! \n", symbol, instructions[i]);
            return TRUE;
        }
    }
    return FALSE;
}