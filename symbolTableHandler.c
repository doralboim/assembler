#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "assembler.h"
#include "constants.h"
#include "instructions.h"
#include "utils.h"

SymbolNode *createNewSymbol(char *symbolName, int IC, SymbolAttribute attribute);
int isLegalSymbol(char *symbol);
int isCommand(char *symbol);


int *insertSymbolToTable(SymbolNode *tableHead, char *symbolName, int IC, SymbolAttribute attribute)
{
    SymbolNode *ptr, *newSymbol;
    int i;
    
    if (isLegalSymbol(symbolName)) return FALSE;

    newSymbol = createNewSymbol(symbolName, IC, attribute);
    if (tableHead->value == 0)
    {
        tableHead = newSymbol;
        return TRUE;
    }

    ptr = tableHead;
    while (ptr->next != NULL)
    {
        if (strcmp(ptr->symbolName, symbolName) == 0)
        {
            if (ptr->attribute1 != EXTERNAL && attribute != EXTERNAL)
            {
                printf("Symbol %s is already exist in the symbolName table!\n", symbolName);
                return FALSE;   
            }

            if (ptr->attribute2 == NULL) ptr->attribute2 = attribute;
        }
        ptr = ptr->next;
    }

    ptr->next = newSymbol;
    
    return TRUE;
}


SymbolNode *createNewSymbol(char *symbolName, int IC, SymbolAttribute attribute)
{
    SymbolNode *newSymbol;

    newSymbol = (SymbolNode *) calloc(0, sizeof(SymbolNode));

    strcpy(newSymbol->symbolName, symbolName);
    newSymbol->value = IC;
    newSymbol->attribute1 = attribute;

    return newSymbol;
}


int isLegalSymbol(char *symbol)
{
    return (isRegister(symbol) || isCommand(symbol));
}


int isCommand(char *symbol)
{
    int i;
    for (i = 0; i < COMMANDS_AMOUNT; i++)
    {
        if (strcmp(symbol, COMMANDS[i].name) == 0)
        {
            printf("symbol %s is an instruction: %s!! \n", symbol, COMMANDS[i]);
            return TRUE;
        }
    }
    return FALSE;
}