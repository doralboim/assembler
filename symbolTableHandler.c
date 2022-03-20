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


int insertSymbolToTable(SymbolNode **tableHead, SymbolNode **tableTail, char *symbolName, int IC, SymbolAttribute attribute)
{
    SymbolNode *ptr, *newSymbol;
    int i;
    printf("inserting symbol to table\n");
    if (isLegalSymbol(symbolName)) return FALSE;

    newSymbol = createNewSymbol(symbolName, IC, attribute);
    if (*tableHead == NULL)
    {
        *tableHead = newSymbol;
        return TRUE;
    }

    if (*tableTail == NULL)
    {
        (*tableHead)->next = newSymbol;
        *tableTail = newSymbol;
        return TRUE;
    }

    (*tableTail)->next = newSymbol;
    *tableTail = newSymbol;
    // newSymbol->next = *tableHead;
    // *tableHead = newSymbol;
    
    
    // ptr = *tableHead;
    // while (ptr->next != NULL)
    // {
    //     // printf("current symbol search is: %s\n", ptr->symbolName);
    //     if (strcmp(ptr->symbolName, symbolName) == 0)
    //     {
    //         /* make attribute2 to EXTERNAL if it's not already defined */
    //         if ((ptr->attribute1 != attribute && attribute != ENTRY &&
    //             ptr->attribute1 != ENTRY && ptr-> attribute2 == NULL) ||
    //             ptr->attribute1 == attribute && attribute != EXTERNAL && attribute != ENTRY)
    //         {
    //             printf("Symbol %s is illegally defined twice!", symbolName);
    //             return FALSE;
    //         }

    //         if (ptr->attribute2 == NULL) ptr->attribute2 = attribute;
    //         break;
    //     }
    //     ptr = ptr->next;
    // }

    // ptr->next = newSymbol;
    printf("Inserted %s to the end of the Symbol table\n", newSymbol->symbolName);
    //printf("before last - %s, last symbol - %s\n", ptr->symbolName, ptr->next->symbolName);
    return TRUE;
}


SymbolNode *createNewSymbol(char *symbolName, int IC, SymbolAttribute attribute)
{
    SymbolNode *newSymbol;

    newSymbol = (SymbolNode *) malloc(sizeof(SymbolNode));

    strcpy(newSymbol->symbolName, symbolName);
    newSymbol->value = IC;
    newSymbol->attribute1 = attribute;
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
        if (strcmp(symbol, commands[i].name) == 0)
        {
            printf("symbol %s is an instruction: %s!! \n", symbol, commands[i]);
            return TRUE;
        }
    }
    return FALSE;
}