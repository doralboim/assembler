#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"
#include "preprocessor.h"

/* parsing macros from source code to processed source code file */
char *processFile(char *fileName)
{
    int isMacro = FALSE;
    char *line, *firstFieldInLine, *processedFileName;
    FILE *originalFp, *processedFp;
    MacroNode *macrosListHead, *currentMacro;

    processedFileName = createProcessedFileName(fileName);
    addFileExtension(fileName);

    if ((originalFp = fopen(fileName, "r")) != NULL &&
        (processedFp = fopen(processedFileName, "w")) != NULL)
    {
        while ((fgets(line, MAX_LINE_SIZE, originalFp)) != NULL)

        {
            sscanf(line, "%s", firstFieldInLine);

            /* reference to macro, insert macro data to the processed file */
            if (fieldInMacros(firstFieldInLine))
            {
                replaceWithMacro(firstFieldInLine, line, processedFp);
            }

            /* macro definition - add it to the macros list */
            else if (strcmp(firstFieldInLine, "macro") == 0)
            {
                isMacro = TRUE;
                currentMacro = addMacro(line);
            }

            /* duration of macro code - add to currentMacro data */
            else if (isMacro && !strcmp(firstFieldInLine, "endm"))
            {
                addDataToMacro(currentMacro, line);
            }

            /* "regular line" without macro reference - copy it to the processed file */
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
