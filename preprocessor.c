#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#include "constants.h"
#include "preprocessor.h"
#include "utils.h"

# define PROCESSED_FILE_NAME (cwd != NULL ? processedFileAbsPath : processedFileName)

void writeMacro(FILE *fp, char *macroName, MacroNode *macrosHead);
int fieldInMacros(char *firstFieldInLine, MacroNode *macroHead);
int addLineToMacro(char *line, MacroNode **macro);
MacroNode *createMacro(char *macroName);
void insertMacroEnd(MacroNode **head, MacroNode *newNode);

/* parsing macros from source code to processed source code file and returns the processed file name*/
char *processFile(char *fileName)
{
    int isMacro = FALSE, isSuccessfulLineAdder;
    char line[MAX_LINE_SIZE], firstFieldInLine[MAX_LINE_SIZE], processedFileName[strlen(fileName) + 3];
    char macroName[MAX_MACRO_NAME];
    int macroData;
    
    FILE *originalFp, *processedFp;
    MacroNode *currentMacro;
    MacroNode *macroHead = NULL;

    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));
    strcat(strcpy(processedFileName, fileName), ".am");
    char processedFileAbsPath[PATH_MAX];
    sprintf(processedFileAbsPath, "%s/%s", cwd, processedFileName);

    char *origFileName = addExetensionToFile(fileName, ".as");

    printf("processed file name is: %s\n", processedFileName);

    if (((originalFp = fopen(origFileName, "r")) == NULL) ||
        ((processedFp = fopen(PROCESSED_FILE_NAME, "w")) == NULL))
    {
        printf("could not open files to preprocess!!\n");
        exit(1);
    }
    else
    {
        while (fgets(line, MAX_LINE_SIZE, originalFp) != NULL)

        {
            sscanf(line, "%s", firstFieldInLine);

            /* reference to macro, insert macro data to the processed file */
            if ((macroData = macroHead == NULL? NULL : fieldInMacros(firstFieldInLine, macroHead)) != NULL)
            {
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
                int j=0;
                for (j=0; j<currentMacro->rowsCount; j++) printf("%d for %s: %s\n", j, currentMacro->name, currentMacro->data + j);
            }

            /* "regular line" without macro reference - copy it to the processed file */
            else
            {
                fputs(line, processedFp);
            }
        } 
        
        fclose(originalFp);
        fclose(processedFp);
    }
    
    return PROCESSED_FILE_NAME;
}

/* writing the given macro content to the file */
void writeMacro(FILE *fp, char *macroName, MacroNode *macrosHead)
{
    MacroNode *ptr = macrosHead;
    while (strcmp(ptr->name, macroName) != 0) ptr = ptr->next;
    int i;
    for (i = 0; i < ptr->rowsCount; i++)
    {
        
        printf("writing: %s\n", ptr->data + (i * MAX_LINE_SIZE));
        fputs(ptr->data + (i * MAX_LINE_SIZE), fp);
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

/* adding new line to the given macro, reallocate dynamic space if no space has left */
int addLineToMacro(char *line, MacroNode **macro)
{
    if ((*macro)->rowsCount >= (*macro)->dataLength - 1)
    {
        printf("create new space for (*macro) %s\n", (*macro)->name);
        (*macro)->data = realloc((*macro)->data, ((*macro)->dataLength + MACRO_LINE_INCREAMENT_FACTOR) * sizeof (char) * MAX_LINE_SIZE);
        if ((*macro)->data == NULL) return FALSE; /* not enough space for (*macro) */
        (*macro)->dataLength += MACRO_LINE_INCREAMENT_FACTOR;
    }
    memset(((*macro)->data + (((*macro)->rowsCount) * MAX_LINE_SIZE)), '\0', MAX_LINE_SIZE);
    strcpy(((*macro)->data) + (((*macro)->rowsCount) * MAX_LINE_SIZE), line);
    ((*macro)->rowsCount)++;
    printf("added %s to %s:%d\n", ((*macro)->data + (*macro)->rowsCount - 1), (*macro)->name, (*macro)->rowsCount);
    return TRUE;
}

/* creating new macro node */
MacroNode *createMacro(char *macroName)
{
    MacroNode *newMacro = (MacroNode *) malloc(sizeof(MacroNode));
    strcpy(newMacro->name, macroName);
    newMacro->data = (char *) malloc(MACRO_LINE_INCREAMENT_FACTOR * sizeof(char) * MAX_LINE_SIZE);
    newMacro->rowsCount = 0;
    newMacro->dataLength = MACRO_LINE_INCREAMENT_FACTOR;
    newMacro->next = NULL;

    return newMacro;
}


// int main(int argc, char const *argv[])
// {
//     // MacroNode *macro = createMacro("mac1");
//     // printf("macro name: %s\n", macroHead->name);
//     // printf("macro rowsCount=%d\n", macro->rowsCount);
//     // addLineToMacro("this line is a test", macro);
//     // printf("macro data: %s\n", macroHead->data);
//     // addLineToMacro("test2", macro);
//     // printf("macro line 2: %s\n", macroHead->data+1);

//     // MacroNode *macro2 = createMacro("mac2");
//     // addLineToMacro("macro 2 first line", macro2);
//     // printf("macro 2 line: %s", macroHead->next->data);

//     char *processedFile;
//     while (--argc > 0)
//     {
//         printf("%s\n", *++argv);
//         processedFile = processFile(*argv);
//     }

//     // MacroNode *ptr = macroHead;
//     // while (ptr != NULL)
//     // {
//     //     printf("data for macro %s is:\n  %s\n", ptr->name, ptr->data);
//     //     ptr = ptr->next;
//     // }

//     return 0;
// }
