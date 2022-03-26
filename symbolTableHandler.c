#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "assembler.h"
#include "constants.h"
#include "utils.h"

SymbolNode *createNewSymbol(char *symbolName, int IC, SymbolAttribute attribute);
int isLegalSymbol(char *symbol);
int isCommand(char *symbol);


int insertSymbolToTable(SymbolNode **tableHead, char *symbolName, int IC, SymbolAttribute attribute)
{
    SymbolNode *ptr, *newSymbol;
    int isInTable = FALSE;
    
    if (isLegalSymbol(symbolName)) return FALSE;

    newSymbol = createNewSymbol(symbolName, IC, attribute);
    if (*tableHead == NULL)
    {
        *tableHead = newSymbol;
        return TRUE;
    }
    
    ptr = *tableHead;
    while (ptr != NULL)
    {
        if (strcmp(ptr->symbolName, symbolName) == 0)
        {
            if (ptr->attribute1 != EMPTY && attribute != ENTRY)
            {
                fprintf(stderr, "Symbol %s illegally defined twice!", symbolName);
                return FALSE;
            }
            if (attribute == ENTRY) ptr->attribute2 = ENTRY;
            else
            {
                ptr->attribute1 = attribute;
                ptr->value = IC;
            }

            isInTable = TRUE;
            break;
        }
        
        if (ptr->next == NULL) break; /* eliminate exiting the list while looping over all symbols */
        ptr = ptr->next;
    }

    if (!isInTable) ptr->next = newSymbol;
    return TRUE;
}


SymbolNode *createNewSymbol(char *symbolName, int IC, SymbolAttribute attribute)
{
    SymbolNode *newSymbol;

    newSymbol = (SymbolNode *) malloc(sizeof(SymbolNode));

    strcpy(newSymbol->symbolName, symbolName);
    newSymbol->value = IC;
    if (attribute == ENTRY)
    {
        newSymbol->attribute1 = EMPTY;
        newSymbol->attribute2 = attribute;
    }
    else
    {
        newSymbol->attribute1 = attribute;
        newSymbol->attribute2 = EMPTY;
    }

    newSymbol->next = NULL;

    return newSymbol;
}


int isLegalSymbol(char *symbol)
{
    return (isRegister(symbol) != -1 || isCommand(symbol));
}


int isCommand(char *symbol)
{
    int i;
    CommandNode commands[] = COMMANDS;
    for (i = 0; i < COMMANDS_AMOUNT; i++)
    {
        if (strcmp(symbol, commands[i].name) == 0) return TRUE;
    }
    return FALSE;
}
