#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "assembler.h"
#include "constants.h"
#include "util.h"

void insertSymbolToTable(char *lineFields, int IC, char *attribute);
int encodeData(char *line, int *dataImage);
int isValidAction(char *action);

int startAssemblerFirstIteration(char *fileName, InstructionNode **instructions)
{
    char *line, *firstLineField, *action;
    int *instructionImage, *dataImage;
    int isError = FALSE, isSymbol = FALSE;
    int IC = 100, DC = 100, encodedData, L;
    int lineArgumentsNum;
    
    FILE *fp;

    SymbolNode *symbolTable = (SymbolNode *) malloc(sizeof(SymbolNode));

    if ((fp = fopen(fileName, 'r')) == NULL)
    {
        printf("File %s does not exist! check your input!", fileName);
        exit(0);
    }

    while(fgets(line, MAX_LINE_SIZE + 1, fp) != NULL)
    {
        lineArgumentsNum = sscanf(line, "%s", firstLineField);

        /* if line is a space or comment - it is ignored */
        if ((strcmp(firstLineField, ";")) || (lineArgumentsNum == 0)) continue;
        
        /* first field is a label - instruction / command is the second field */
        if (strcmp(firstLineField[strlen(firstLineField) - 1], ":") == 0)
        {
            isSymbol = TRUE;
            lineArgumentsNum = sscanf(line, "%s", firstLineField, action);
        }
        /* no symbol - instruction / command is the first field */
        else action = firstLineField;

        /* is current instruction for data storage */
        if ((strcmp(action, ".data") == 0) || (strcmp(action, ".string") == 0))
        {
            if (isSymbol) insertSymbolToTable(firstLineField, IC, DATA);
            encodedData = encodeData(line, dataImage);
            DC += encodedData;
        }

        /* if current instruction of type extern inserts it to the symbol table */
        else if (strcmp(action, ".extern") == 0) insertSymbolToTable(firstLineField, 0, EXTERNAL);

        /* if current instruction of type entry it would be handled in the second iteration */
        else if (strcmp(action, ".entry") == 0) continue;

        /* line is a command */
        else
        {
            if (isSymbol) insertSymbolToTable(firstLineField, IC, CODE);
            if (!isValidAction(action))
            {
                printf("Invalid action name - got %s", action);
                exit(1);
            }
            L = getActionWords(line, isSymbol);
        }

        isSymbol = FALSE;
    }
}

/* Insert a symbol to the symbol table, after validation - is the symbol legitimate and does not already exist in the table */
void insertSymbolToTable(char *symbol, int IC, char *attribute)
{
    return ;
}