#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assemblerConstants.h"
#include "preprocessor.h"

/* parsing macros from source code to processed source code file */
char *processFile(char *fileName)
{
    int isMacro = FALSE;
    char *line, *firstFieldInLine, *processedFileName;
    FILE *originalFp, *processedFp;
    MacroNode *macrosStart, *tail;

    processedFileName = createProcessedFileName(fileName);
    addFileExtension(fileName);

    if ((originalFp = fopen(fileName, "r")) != NULL &&
        (processedFp = fopen(processedFileName, "w")) != NULL)
    {
        while ((fgets(line, maxLineSize, originalFp)) != NULL)

        {
            sscanf(line, "%s", firstFieldInLine);

            if (fieldInMacros(firstFieldInLine))
            {
                writeMacro(firstFieldInLine, processedFp);
            }
            else if (strcmp(firstFieldInLine, "mcro") == 0)
            {
                isMacro = TRUE;
                addMacro(line);
            }
            else
            {
                fputs(line, processedFp);
            }
        } 
    }
    // char *fileWithExtension;

    // fileLength = strlen(fileName);
    // fileWithExtension = malloc((fileLength * sizeof(char)) + 3);

}
