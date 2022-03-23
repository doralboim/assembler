#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"
#include "preprocessor.h"

void writeMacro(FILE *fp, char *macroName, MacroNode *macrosHead);
int fieldInMacros(char *firstFieldInLine, MacroNode *macroHead);
int addLineToMacro(char *line, MacroNode **macro);
MacroNode *createMacro(char *macroName);
void insertMacroEnd(MacroNode **head, MacroNode *newNode);
// void replaceWithMacro(char *firstFieldInLine, char *line, FILE *processedFp);

/* parsing macros from source code to processed source code file */
char *processFile(char *fileName)
{
    int isMacro = FALSE, isSuccessfulLineAdder;
    char line[MAX_LINE_SIZE], firstFieldInLine[MAX_LINE_SIZE], processedFileName[strlen(fileName) + 3];
    char macroName[MAX_MACRO_NAME];
    int macroData;
    
    FILE *originalFp, *processedFp;
    MacroNode *currentMacro;
    MacroNode *macroHead = NULL;

    strcat(strcpy(processedFileName, fileName), ".am");
    printf("processed file name is: %s\n", processedFileName);

    if (((originalFp = fopen(fileName, "r")) == NULL) ||
        ((processedFp = fopen(processedFileName, "w")) == NULL))
    {
        printf("could not open files to preprocess!!\n");
        exit(1);
    }
    else
    {
        while (fgets(line, MAX_LINE_SIZE, originalFp) != NULL)

        {
            printf("\nstart working on line: %s\n", line);
            sscanf(line, "%s", firstFieldInLine);

            /* reference to macro, insert macro data to the processed file */
            if ((macroData = macroHead == NULL? NULL : fieldInMacros(firstFieldInLine, macroHead)) != NULL)
            {
                printf("write macro to file!\n");
                writeMacro(processedFp, firstFieldInLine, macroHead);
            }

            /* macro definition - add it to the macros list */
            else if (strcmp(firstFieldInLine, START_MACRO_TERM) == 0)
            {
                isMacro = TRUE;
                sscanf(line, "%s%s", firstFieldInLine, macroName);
                currentMacro = createMacro(macroName);
            }

            /* duration of macro code - add to currentMacro data */
            else if (isMacro && strcmp(firstFieldInLine, END_MACRO_TERM) != 0)
            {
                isSuccessfulLineAdder = addLineToMacro(line, &currentMacro);
            }
            else if (strcmp(firstFieldInLine, END_MACRO_TERM) == 0)
            {
                isMacro = FALSE;
                if (macroHead == NULL) macroHead = currentMacro;
                else insertMacroEnd(&macroHead, currentMacro);
            }

            /* "regular line" without macro reference - copy it to the processed file */
            else
            {
                printf("write line %s to file %s\n", line, processedFileName);
                fputs(line, processedFp);
            }
        } 
        
        fclose(originalFp);
        fclose(processedFp);
    }

    // MacroNode *ptr = macroHead;
    // size_t dataLength;
    // while (ptr != NULL)
    // {
    //     printf("macro %s:\n", ptr->name);
    //     dataLength = sizeof(ptr->data);
    //     int i;
    //     for (i = 0; i < dataLength; i++) printf("\t%s", ptr->data + i);
    // }
    printf("end of program!\n");
    return processedFileName;

}

/* writing the given macro content to the file */
void writeMacro(FILE *fp, char *macroName, MacroNode *macrosHead)
{
    MacroNode *ptr = macrosHead;
    while (strcmp(ptr->name, macroName) != 0) ptr = ptr->next;
    int i;
    for (i = 0; i < ptr->rowsCount - 1; i++)
    {
        
        printf("writing: %s\n", (ptr->data + i));
        fputs((ptr->data + i), fp);
    }
    printf("Successfully wrote macro %s to file\n", macroName);
}

/* Insert new macro node to the last place the the macros table */
void insertMacroEnd(MacroNode **head, MacroNode *newNode)
{
    MacroNode *ptr = *head;
    while (ptr->next != NULL) ptr = ptr->next;
    ptr->next = newNode;
}

/* Serach for firstFieldInLine in the macros table and return TRUE if exists, otherwise return FALSE */
int fieldInMacros(char *firstFieldInLine, MacroNode *macroHead)
{
    MacroNode *ptr = macroHead;
    while(ptr != NULL)
    {
        if (strcmp(firstFieldInLine, ptr->name) == 0) return TRUE;
        ptr = ptr->next;
    }
    return FALSE;
}

int addLineToMacro(char *line, MacroNode **macro)
{
    printf("starting to add line to %s\n", (*macro)->name);
    if ((*macro)->rowsCount >= (*macro)->dataLength - 1)
    {
        printf("create new space for (*macro) %s\n", (*macro)->name);
        (*macro)->data = realloc((*macro)->data, ((*macro)->dataLength + MACRO_LINE_INCREAMENT_FACTOR) * sizeof (char) * MAX_LINE_SIZE);
        if ((*macro)->data == NULL) return FALSE; /* not enough space for (*macro) */
        (*macro)->dataLength += MACRO_LINE_INCREAMENT_FACTOR;
    }
    memset(((*macro)->data + (*macro)->rowsCount), '\0', MAX_LINE_SIZE);
    strcpy(((*macro)->data + (*macro)->rowsCount), line);
    ((*macro)->rowsCount)++;
    printf("added %s to %s\n", ((*macro)->data + (*macro)->rowsCount - 1), (*macro)->name);
    return TRUE;
}

MacroNode *createMacro(char *macroName)
{
    MacroNode *newMacro = (MacroNode *) malloc(sizeof(MacroNode));
    //memset(newMacro->name, '\0', strlen(newMacro->name));
    //newMacro->name = (char *) malloc(sizeof(macroName) * sizeof(char) + 1);
    //*(newMacro->name) = *macroName;
    strcpy(newMacro->name, macroName);
    newMacro->data = (char **) malloc(MACRO_LINE_INCREAMENT_FACTOR * sizeof(char) * MAX_LINE_SIZE);
    newMacro->rowsCount = 0;
    newMacro->dataLength = MACRO_LINE_INCREAMENT_FACTOR;
    newMacro->next = NULL;

    return newMacro;
}


int main(int argc, char const *argv[])
{
    // MacroNode *macro = createMacro("mac1");
    // printf("macro name: %s\n", macroHead->name);
    // printf("macro rowsCount=%d\n", macro->rowsCount);
    // addLineToMacro("this line is a test", macro);
    // printf("macro data: %s\n", macroHead->data);
    // addLineToMacro("test2", macro);
    // printf("macro line 2: %s\n", macroHead->data+1);

    // MacroNode *macro2 = createMacro("mac2");
    // addLineToMacro("macro 2 first line", macro2);
    // printf("macro 2 line: %s", macroHead->next->data);

    char *processedFile;
    while (--argc > 0)
    {
        printf("%s\n", *++argv);
        processedFile = processFile(*argv);
    }

    // MacroNode *ptr = macroHead;
    // while (ptr != NULL)
    // {
    //     printf("data for macro %s is:\n  %s\n", ptr->name, ptr->data);
    //     ptr = ptr->next;
    // }

    return 0;
}
