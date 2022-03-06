#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "assembler.h"
#include "constants.h"
#include "utils.h"

int computeCommandWords(char line[], int isSymbol, char command[]);

IterationsData *startAssemblerFirstIteration(char *fileName)
{
    char line[MAX_LINE_SIZE], firstLineField[MAX_LINE_SIZE], action[MAX_LINE_SIZE], *symbolName;
    int *instructionImage, *dataImage;
    int isError = FALSE, isSymbol;
    int IC = 100, DC = 100, encodedData, L;
    int lineArgumentsNum;
    
    FILE *fp;

    SymbolNode *symbolTable = (SymbolNode *) malloc(sizeof(SymbolNode));

    if ((fp = fopen(fileName, 'r')) == NULL)
    {
        printf("File %s does not exist! check your input!", fileName);
        exit(0);
    }

    while(fgets(line, MAX_LINE_SIZE + 1, fp) != NULL)
    {
        lineArgumentsNum = sscanf(line, "%s", firstLineField);

        isSymbol = FALSE;

        /* if line is a space or comment - it is ignored */
        if ((lineArgumentsNum == 0) || *line == ';') continue;
        
        /* first field is a label - instruction / command is the second field */
        if (strcmp(firstLineField[strlen(firstLineField) - 1], ":") == 0)
        {
            strncpy(symbolName, firstLineField, strlen(firstLineField) - 2);
            isSymbol = TRUE;
            lineArgumentsNum = sscanf(line, "%s", firstLineField, action);
        }
        /* no symbolName - instruction / command is the first field */
        else strcpy(action, firstLineField);

        /* is current instruction for data storage */
        if (strcmp(action, ".data") || strcmp(action, ".string"))
        {
            if (isSymbol) insertSymbolToTable(symbolTable, symbolName, IC, DATA);
            DC = encodeData(isSymbol? *(line + strlen(symbolName)): line, dataImage, DC);
        }

        /* if current instruction of type extern inserts it to the symbol table */
        else if (strcmp(action, ".extern") == 0) insertSymbolToTable(symbolTable, firstLineField, 0, EXTERNAL);

        /* if current instruction of type entry it would be handled in the second iteration */
        else if (strcmp(action, ".entry") == 0) continue;

        /* line is a command */
        else
        {
            if (isSymbol) insertSymbolToTable(symbolTable, firstLineField, IC, CODE);
            if (!isValidAction(action))
            {
                printf("Invalid action name - got %s", action);
                isError = TRUE;
                continue;
            }
            L = computeCommandWords(line, isSymbol, action);
        }
    }

    if (isError == TRUE)
    {
        printf("Errors occured in the first assembler iteration, program stops!");
        exit(1);
    }

    IterationsData *response = (IterationsData *) malloc(sizeof(IterationsData));
    response->symbolTable = symbolTable;
    response->instructionImage = instructionImage;
    response->dataImage = dataImage;

    return response;

}


int computeCommandWords(char line[], int isSymbol, char command[])
{
    /* stop & rts get no operands */
    printf("condition is: %d\n", strcmp(command, "rts") == 0|| strcmp(command, "stop") == 0);
    if (strcmp(command, "rts") == 0 || strcmp(command, "stop") == 0) return 1;
    
    int words = 2, commas, allowedOperands;
    commas = getNumberOfCommas(line);
    printf("commas number is: %d\n", commas);
    if (commas > 1)
    {
        printf("Illegal number of operands, expected max of 2 and got %d\n", commas - 1);
        return -1;
    }

    /* 2 operands - checking the source operand and adding its words is required */
    allowedOperands = getNumberOfAllowedOperandsByCommand(command);
    if (commas == 1)
    {
        if (allowedOperands != 2)
        {
            printf("Command %s must get %d operands but got 2 for line:\n%s\n!!", command, allowedOperands, line);
        }
        char *sourceOperand;
        uint8_t sourceAddressMethod;
        
        sourceOperand = strtok(line + strlen(command), ",");
        sscanf(sourceOperand, "%s", sourceOperand);
        printf("source operand is: %s\n", sourceOperand);

        sourceAddressMethod = addressingMethodByOperand(sourceOperand);
        if (sourceAddressMethod == -1) return -1;
        if (isAllowedAddressingMethodByCommand(command, sourceAddressMethod, SOURCE)) words += getWordsNumByAdressMethod(sourceAddressMethod);
        else
        {
            printf("Invalid addressing method for source operand - line %s!", line);
            return -1;
        }
    }
    
    char *desOperand;
    uint8_t desAddressMethod;

    /* Zero operands command - if a legal non-operands command returns 1, otherwise return -1 (error) */
    desOperand = commas == 1? strtok(NULL, ",") : strtok(line + strlen(command), ",");
    if (*desOperand == NULL)
    {
        if (allowedOperands != 0)
        {
            printf("Command %s must get %d operands but got 0 for line:\n%s!!\n", command, allowedOperands, line);
            return -1;
        }
        return 1;
    }

    /* check if actions is allowed with 1 operand */
    if (commas == 0 && allowedOperands != 1)
    {
        printf("Command %s must get %d operands but got 1 for line:\n%s!!\n", command, allowedOperands, line);
        return -1;
    }

    /* destination operand operations are identical to 1 and 2 operands' actions */
    sscanf(desOperand, "%s", desOperand);
    printf("des operand is: %s\n", desOperand);

    desAddressMethod = addressingMethodByOperand(desOperand);
    if (desAddressMethod == -1) return -1;
    if (isAllowedAddressingMethodByCommand(command, desAddressMethod, DESTINATION)) words += getWordsNumByAdressMethod(desAddressMethod);
    else
    {
        printf("Invalid addressing method for destination operand - line %s!", line);
        return -1;
    }

    return words;
}

int main(int argc, char const *argv[])
{
    char test[30] = "mov    x[r12]     ";
    int res = computeCommandWords(test, FALSE, "mov"); // isAllowedAddressingMethodByCommand("jsr", DIRECT_ADDRESING, SOURCE);
    printf("result is: %d\n", res);
    return 0;
}