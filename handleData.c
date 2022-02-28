#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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

void encodeData(char line[], int *dataImage, int *DC)
{
    char *instruction, *token, *onlyData, delimiter[2] = ",";
    int data, intToken, *string;
    
    if (sscanf(line, "%s", instruction))
    {
        if (strcmp(instruction, ".data") == 0)
        {
            token = strtok(&line[strlen(instruction)], delimiter);
            while (token != NULL)
            {
                {
                    intToken = atoi(token);
                    *(dataImage + (*DC)++) = decimalToBinary(intToken);
                    printf("token is %s\n", token);
                    token = strtok(NULL, delimiter);
                }
            }
        }
        else
            {
                // printf("%s\n", line);
                sscanf(line, "%s%s", instruction, &string);
                printf("string var is: %s", (&string)[1]);
            //     int i;
            //     for (i = 1; i < strlen(&string); i++)
            //     {
            //         printf("char is: %c\n", *(string + i));
            //     }
            }
    }

}

int main(int argc, char const *argv[])
{
    int *data, dc;
    // char delimiter = ",";
    dc = 5;
    data = (int *) calloc(0, sizeof(int) * 100);
    char line[30] = ".string \"abdckjt\"";
    encodeData(line, data, &dc);

    int i;
    for (i = 0; i < 10; i++) printf("iteration number %d - value is %d\n", i, *(data + i));

    return 1;
}
