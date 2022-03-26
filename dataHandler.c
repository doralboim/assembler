#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"

int decimalToBinary(int n) {
  int bin = 0;
  int rem, i = 1;

  while (n!=0) {
    rem = n % 2;
    n /= 2;
    bin += rem * i;
    i *= 10;
  }

  return bin;
}

/* encode numnbers and characters and insert to the data image, returning the increased DC */
int encodeData(char line[], int dataImage[], int dc)
{
    char *instruction, *token, *stringData;
    int intToken, i = 1;
    printf("dc is: %d\n", dc);
    
    if ((instruction = strstr(line, DATA_INSTRUCTION)) != NULL)
    {
        printf("instruction %s is data\n", instruction);
        token = strtok(instruction + strlen(DATA_INSTRUCTION), ",");
        while (token != NULL)
        {
            {
                sscanf(token, "%s", token);
                intToken = atoi(token);
                dataImage[dc++] = intToken;
                token = strtok(NULL, ",");
            }
        }
    }
    else
        {
            instruction = strstr(line, STRING_INSTRUCTION);
            stringData = strstr(line, "\"");
            while ((dataImage[dc++] = *(stringData + i++)) != '"') ;
            dataImage[dc - 1] = 0;
        }
    
    return dc;
}

