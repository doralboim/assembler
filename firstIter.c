#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "assembler.h"
#include "constants.h"
#include "dataHandler.h"
#include "utils.h"
#include "instructions.h"

int computeCommandWords(char line[], int ic, int isSymbol, char command[], InstructionData **instructionDataHead);
void *updateDataSymbols(SymbolNode **symbolTable, int finalIC);
operandData *analyseOperand(char line[], char command[], char *operand, OperandType operandType, int *words);
void printData(IterationsData *response);

IterationsData *startAssemblerFirstIteration(char *fileName)
{
    char line[MAX_LINE_SIZE + 1], firstLineField[MAX_LINE_SIZE], action[MAX_LINE_SIZE], *symbolName;
    int *instructionImage, *dataImage;
    int isError = FALSE, isSymbol;
    int IC = 100, DC = 0, encodedData, L;
    int lineArgumentsNum, isSymbolInsertedToTable, countLines = 1;
    
    FILE *fp;

    dataImage = (int *) malloc(MEMORY_SIZE * sizeof(int));
    SymbolNode *symbolTableHead = NULL;
    SymbolNode *symbolTableTail = NULL;
    InstructionData *instructionDataHead = (InstructionData *) malloc(sizeof(InstructionData));

    if ((fp = fopen(fileName, "r")) == NULL)
    {
        printf("File %s does not exist! check your input!", fileName);
        exit(0);
    }

    while(fgets(line, MAX_LINE_SIZE + 1, fp) != NULL)
    {
        printf("\n\n\n%d - %s\n", countLines++, line);
        lineArgumentsNum = sscanf(line, "%s", firstLineField);

        isSymbol = FALSE;

        /* if line is a space or comment - it is ignored */
        if ((lineArgumentsNum == 0) || *line == ';') continue;
        
        /* first field is a label - instruction / command is the second field */
        if (strcmp(&firstLineField[strlen(firstLineField) - 1], ":") == 0)
        {
            memset(symbolName, '\0', strlen(firstLineField));
            strncpy(symbolName, firstLineField, strlen(firstLineField) - 1);
            printf("symbol name is : %s\n", symbolName);
            isSymbol = TRUE;
            lineArgumentsNum = sscanf(line, "%s%s", firstLineField, action);
        }
        /* no symbolName - instruction / command is the first field */
        else strcpy(action, firstLineField);
        printf("action is: %s\n", action);

        /* is current instruction for data storage */
        if (strcmp(action, ".data") == 0|| strcmp(action, ".string") == 0)
        {
            if (isSymbol) insertSymbolToTable(&symbolTableHead, &symbolTableTail, symbolName, DC, DATA);
            DC = encodeData(line, dataImage, DC);
        }
        /* if current instruction of type extern inserts it to the symbol table */
        else if (strcmp(action, ".extern") == 0)
        {
            if (isSymbol) insertSymbolToTable(symbolTableHead, &symbolTableTail, firstLineField, 0, EXTERNAL);
            continue;
        }

        /* if current instruction of type entry it would be handled in the second iteration */
        else if (strcmp(action, ".entry") == 0)
        {
            if (isSymbol) printf("Need to insert symbol to table\n");
            else continue;
        }

        /* line is a command */
        else
        {
            if (!isValidCommand(action))
            {
                printf("Invalid action name - got %s\n", action);
                isError = TRUE;
                continue;
            }
            
            if (isSymbol)
            {
                isSymbolInsertedToTable = insertSymbolToTable(&symbolTableHead, &symbolTableTail, symbolName, IC, CODE);
                printf("insert command to symbol table result : %d\n", isSymbolInsertedToTable);
                if (!isSymbolInsertedToTable)
                {
                    isError = TRUE;
                    continue;
                }
            }

            L = computeCommandWords(line, IC, isSymbol, action, &instructionDataHead);
            IC += L;
            // if (IC > MEMORY_SIZE)
            // {
            //     printf("Memory exeeds available memory!");
            //     isError = TRUE;
            // }
        }
    }

    if (isError == TRUE)
    {
        printf("Errors occured in the first assembler iteration, program stops!");
        exit(1);
    }

    updateDataSymbols(&symbolTableHead, IC);

    IterationsData *response = (IterationsData *) malloc(sizeof(IterationsData));
    response->symbolTable = NULL;
    response->instructionData = instructionDataHead;
    response->dataImage = dataImage;
    
    // printData(response);
    fclose(fp);
    return response;

}

void printData(IterationsData *response)
{
    SymbolNode *symPtr = response->symbolTable;
    InstructionData *insPtr = response->instructionData;
    int i;
    for (i=0; i < 10; i++) printf("data image on place %d is: %d\n", i, (response->dataImage)[i]);

    while (symPtr != NULL)
    {
        printf("symbol: %s %d %d\n", symPtr->symbolName, symPtr->value, symPtr->attribute1);
        symPtr = symPtr->next;
    }

    while (insPtr != NULL)
    {
        printf("instructions: %s %d %d %s %s\n", insPtr->command, insPtr->IC, insPtr->words, insPtr->sourceOperand->label, insPtr->destinationOperand->label);
        insPtr = insPtr->next;
    }
}

int computeCommandWords(char line[], int ic, int isSymbol, char command[], InstructionData **instructionDataHead)
{
    /* stop & rts get no operands */
    if (strcmp(command, "rts") == 0 || strcmp(command, "stop") == 0) return 1;
    
    char *lineWithoutSymbol = strstr(line, command);
    int words = 2, commas, allowedOperands;
    commas = getNumberOfCommas(lineWithoutSymbol);
    
    /* more than 1 comma means more than 2 operands which is iillegal */
    if (commas > 1)
    {
        printf("Illegal number of operands, expected max of 2 and got %d\n", commas - 1);
        return -1;
    }

    /* 2 operands - checking the source operand and adding its words is required */
    allowedOperands = getNumberOfAllowedOperandsByCommand(command);
    char *sourceOperand;
    operandData *sourceOperandData = NULL;
    if (commas == 1)
    {
        if (allowedOperands != 2)
        {
            printf("Command %s must get %d operands but got 2 for lineWithoutSymbol:\n%s\n!!", command, allowedOperands, lineWithoutSymbol);
        }
        
        sourceOperand = strtok(lineWithoutSymbol + strlen(command) + 1, ",");
        printf("source operand is: %s\n", sourceOperand);
        sourceOperandData = analyseOperand(lineWithoutSymbol, command, sourceOperand, SOURCE, &words);
    }
    
    char *desOperand;
    uint8_t desAddressMethod;

    /* Zero operands command - if a legal non-operands command returns 1, otherwise return -1 (error) */
    desOperand = commas == 1? strtok(NULL, ",") : strtok(lineWithoutSymbol + strlen(command) + 1, ",");
    sscanf(desOperand, "%s", desOperand);
    printf("Destination operand is: %s\n", desOperand);
    if (*desOperand == NULL)
    {
        if (allowedOperands != 0)
        {
            printf("Command %s must get %d operands but got 0 for lineWithoutSymbol:\n%s!!\n", command, allowedOperands, lineWithoutSymbol);
            return -1;
        }
        return 1;
    }

    /* check if actions is allowed with 1 operand */
    if (commas == 0 && allowedOperands != 1)
    {
        printf("Command %s must get %d operands but got 1 for lineWithoutSymbol:\n%s!!\n", command, allowedOperands, lineWithoutSymbol);
        return -1;
    }

    /* destination operand operations are identical to 1 and 2 operands' actions */
    printf("destination operand is %s\n", desOperand);
    operandData *destinationOperandData = analyseOperand(lineWithoutSymbol, command, desOperand, DESTINATION, &words);
    printf("analysis succeeded\n");

    //InstructionData *newHead = (InstructionData *) malloc(sizeof(InstructionData));
    saveInstructionData(command, ic, words, sourceOperandData, destinationOperandData, instructionDataHead);

    return words;
}

/* return the operand data - register / value / label and increase the words count accordingly */
operandData *analyseOperand(char line[], char command[], char *operand, OperandType operandType, int *words)
{
    operandData *opData;
    sscanf(operand, "%s", operand);
    printf("start analysing operand %s\n", operand);

    opData = addressingMethodByOperand(operand);
    printf("Operand data is: label: %s, register num: %d\n", opData->label, opData->registerNum);
    if (!isAllowedAddressingMethodByCommand(command, opData->addressingMethod, operandType))
    {
        printf("Invalid addressing method for source operand - line %s!", line);
        opData->addressingMethod = 0;
        return opData;
    }
    
    int wordsByAdressing = getWordsNumByAdressMethod(opData->addressingMethod);
    *words += wordsByAdressing;
    printf("words for %s - %d\n", command, *words);
    return opData;
}

/* update data symbols by adding IC + 100 to every address */
void *updateDataSymbols(SymbolNode **symbolTable, int finalIC)
{
    SymbolNode *ptr = *symbolTable;
    while (ptr != NULL)
    {
        if (ptr->attribute1 == DATA) ptr->value += finalIC;
        ptr = ptr->next;
    }
}

// int main(int argc, char const *argv[])
// {
//     IterationsData *data;// = (IterationsData *) malloc(sizeof(IterationsData));
//     printf("%s\n", argv[1]);
//     printf("%s\n", argv[1]);
//     data = startAssemblerFirstIteration(argv[1]);
//     // while (--argc > 0)
//     // {
//     //     printf("%s\n", *++argv);
//     //     data = startAssemblerFirstIteration(argv[1]);
//     // }
//     return 0;
// }
