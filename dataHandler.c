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
    char instruction[10], *token;
    int result, intToken;
    
    if ((result = sscanf(line, "%s", instruction)) > 0)
    {
        if (strcmp(instruction, ".data") == 0)
        {
            token = strtok(&line[strlen(instruction)], ",");
            while (token != NULL)
            {
                {
                    intToken = atoi(token);
                    dataImage[dc++] = decimalToBinary(intToken);
                    token = strtok(NULL, ",");
                }
            }
        }
        else
            {
                char stringData[10];
                int ch;
                result = sscanf(line, "%s%s", instruction, stringData);
                //printf("string var is: %s, length is %d\n", stringData, strlen(stringData));
                int i;
                for (i=1; i < strlen(stringData) - 1; i++)
                {
                    sscanf(&stringData[i], "%c",&ch);
                    dataImage[dc++] = decimalToBinary(ch);
                    // printf("char is: %d\n", decimalToBinary(ch));
                }
                dataImage[dc++] = 0;
            }
    }
    return dc;
}

int main(int argc, char const *argv[])
{
    int data[100];
    int dc = 0, res;
    // char delimiter = ",";
    // printf("dc is: %d\n", dc);
    //data = (int *) calloc(0, sizeof(int) * 100);
    char line[30] = ".data 124,64,346"; // ".string \"abdckjt\"";
    char line2[30] = ".string \"abdckjt\"";
    res=encodeData(line, data, dc);

    int i;
    for (i = 0; i < res; i++) printf("iteration number %d - value is %d\n", i, *(data + i));

    return 1;
}
