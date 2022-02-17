#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "assemblerConstants.h"

 void insertSymbolToTable(char *lineFields, int IC, char *attribute);
 int encodeData(char *lineFields[], int *dataImage);
 int isValidAction(char *action);

int startAssemblerFirstIteration(char *fileName)
{
    char *line, *lineFields[5], *action;
    int *instructionImage, *dataImage;
    int isError = FALSE, isSymbol = FALSE;
    int IC = 100, DC = 100, encodedData;
    int lineArgumentsNum;
    FILE *fp;

    if ((fp = fopen(fileName, 'r')) == NULL)
    {
        printf("File %s does not exist! check your input!", fileName);
        exit(0);
    }

    while(fgets(line, maxLineSize + 1, fp) != NULL)
    {
        lineArgumentsNum = sscanf(line, "%s%s%s%s%s", lineFields[0], lineFields[1], lineFields[2], lineFields[3], lineFields[4]);

        /* if line is a space or comment - it is ignored */
        if ((strcmp(lineFields[0], ";")) || (lineArgumentsNum == 0)) continue;
        
        /* first field is a label - instruction / command is the second field */
        if (strcmp(lineFields[0][-1], ":") == 0)
        {
            isSymbol = TRUE;
            action = lineFields[1];
        }
        /* no symbol - instruction / command is the first field */
        else action = lineFields[0];

        /* is current instruction for data storage */
        if ((strcmp(action, ".data") == 0) || (strcmp(action, ".string") == 0))
        {
            if (isSymbol) insertSymbolToTable(lineFields[0], IC, "data");
            encodedData = encodeData(lineFields, dataImage);
            DC += encodedData;
        }

        /* if current instruction of type extern inserts it to the symbol table */
        else if (strcmp(action, ".extern") == 0) insertSymbolToTable(lineFields[0], 0, "extern");

        /* if current instruction of type entry it would be handled in the second iteration */
        else if (strcmp(lineFields[1], ".entry") == 0) continue;

        /* line is a command */
        else
        {
            if (isSymbol) insertSymbolToTable(lineFields[0], IC, "code");
            if (!isValidAction(action));
        }

        isSymbol = FALSE;
    }
}


/* Insert a symbol to the symbol table, after validation - is the symbol legitimate and does not already exist in the table */
void insertSymbolToTable(char *symbol, int IC, char *attribute)
{
    return ;
}