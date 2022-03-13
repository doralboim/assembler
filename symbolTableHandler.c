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


int insertSymbolToTable(SymbolNode *tableHead, char *symbolName, int IC, SymbolAttribute attribute)
{
    SymbolNode *ptr, *newSymbol;
    int i;
    printf("inserting symbol to table\n");
    if (isLegalSymbol(symbolName)) return FALSE;

    printf("after checking if symbol is legal\n");
    newSymbol = createNewSymbol(symbolName, IC, attribute);
    if (tableHead->value == 0)
    {
        tableHead = newSymbol;
        printf("end of inserting symbol to table - %s\n", symbolName);
        return TRUE;
    }

    ptr = tableHead;
    while (ptr->next != NULL)
    {
        if (strcmp(ptr->symbolName, symbolName) == 0)
        {
            /* make attribute2 to EXTERNAL if it's not already defined */
            if ((ptr->attribute1 != attribute && attribute != ENTRY &&
                ptr->attribute1 != ENTRY && ptr-> attribute2 == NULL) ||
                ptr->attribute1 == attribute && attribute != EXTERNAL && attribute != ENTRY)
            {
                printf("Symbol %s is illegally defined twice!", symbolName);
                return FALSE;
            }

            if (ptr->attribute2 == NULL) ptr->attribute2 = attribute;
            break;
        }
        ptr = ptr->next;
    }

    ptr->next = newSymbol;
    printf("end of inserting symbol to table\n");
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
    return (isRegister(symbol) != -1 || isCommand(symbol));
}


int isCommand(char *symbol)
{
    int i;
    CommandNode *commands = machine_commands();
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