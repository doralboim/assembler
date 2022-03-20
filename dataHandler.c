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
    char *instruction, *token, symbol[MAX_SYMBOL_NAME_LENGTH];
    int result, intToken;
    printf("dc is: %d\n", dc);
    
    // if ((result = isSymbol? sscanf(line, "%s%s", symbol, instruction) : sscanf(line, "%s", instruction)) > 0)
    
    if ((instruction = strstr(line, DATA_INSTRUCTION)) != NULL)
    {
        printf("instruction %s is data\n", instruction);
        token = strtok(instruction + strlen(DATA_INSTRUCTION), ",");
        while (token != NULL)
        {
            {
                sscanf(token, "%s", token);
                intToken = atoi(token);
                dataImage[dc++] = decimalToBinary(intToken);
                //printf("dataImage[%d] = %d\n", dc - 1, dataImage[dc - 1]);
                token = strtok(NULL, ",");
            }
        }
    }
    else
        {
            instruction = strstr(line, STRING_INSTRUCTION);
            printf("instruction %s is string\n", instruction);
            //char stringData[10];
            unsigned char ch;
            char *stringData = strstr(line, "\"");
            int i = 1;
            while ((dataImage[dc++] = *(stringData + i++)) != '"') ;
            // printf("string var is: %s, length is %d\n", stringData, strlen(stringData));
            // int i;
            // for (i=1; i < strlen(stringData) - 1; i++)
            // {
            //     sscanf(&stringData[i], "%c",&ch);
            //     dataImage[dc++] = ch; //decimalToBinary(ch);
            //     printf("value for char %c is is: %u\n", ch, ch);
            //     printf("data is: %d\n", dataImage[dc - 1]);
            // }
            dataImage[dc++] = 0;
        }
    
    return dc;
}

// int main(int argc, char const *argv[])
// {
//     int data[100];
//     int dc = 0, res;
//     // char delimiter = ",";
//     // printf("dc is: %d\n", dc);
//     //data = (int *) calloc(0, sizeof(int) * 100);
//     char line[30] = ".data 124,64,346"; // ".string \"abdckjt\"";
//     char line2[30] = ".string \"abdckjt\"";
//     res=encodeData(line, data, dc);

//     int i;
//     for (i = 0; i < res; i++) printf("iteration number %d - value is %d\n", i, *(data + i));

//     return 1;
// }
