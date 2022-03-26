#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MIN_DATA_VALUE ((-1) * pow(2.0, MEMORY_PARTITION))
#define MAX_DATA_VALUE (pow(2.0, MEMORY_PARTITION) - 1)

#include "constants.h"

/* encode numnbers and characters and insert to the data image, returning the increased DC */
long encodeData(char line[], long dataImage[], int dc)
{
    char *instruction, *token, *stringData, *residual;
    int i = 1, countElements = 0;
    long longToken;
    
    if ((instruction = strstr(line, DATA_INSTRUCTION)) != NULL)
    {
        token = strtok(instruction + strlen(DATA_INSTRUCTION), ",");
        while (token != NULL)
        {
            {
                sscanf(token, "%s", token);
                longToken = strtol(token, residual, 10);
                if (longToken > MAX_DATA_VALUE || longToken < MIN_DATA_VALUE)
                {
                    fprintf(stderr, "Data value exceeds exceptable range. Allowed range is %ld - %ld but got %ld", MIN_DATA_VALUE, MAX_DATA_VALUE, longToken);
                    return -1;                    
                }
                dataImage[dc++] = longToken;
                countElements++;
                token = strtok(NULL, ",");
            }
        }
    }
    else
        {
            instruction = strstr(line, STRING_INSTRUCTION);
            stringData = strstr(line, "\"");
            while ((dataImage[dc++] = *(stringData + i++)) != '"') countElements++;
            dataImage[dc - 1] = 0;
        }
    
    return countElements > 0 ? dc : -1;
}

