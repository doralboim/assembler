#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "assembler.h"
#include "constants.h"
#include "dataHandler.h"
#include "utils.h"
#include "instructions.h"

int computeCommandWords(char line[], int ic, int isSymbol, char command[], InstructionData *instructionDataHead);
void *updateDataSymbols(SymbolNode *symbolTable, int finalIC);
operandData *analyseOperand(char line[], char command[], char *operand, OperandType operandType, int *words);
void printData(IterationsData *response);

IterationsData *startAssemblerFirstIteration(char *fileName)
{
    printf("start function");
    char line[MAX_LINE_SIZE + 1], firstLineField[MAX_LINE_SIZE], action[MAX_LINE_SIZE], *symbolName;
    int *instructionImage, *dataImage;
    int isError = FALSE, isSymbol;
    int IC = 100, DC = 100, encodedData, L;
    int lineArgumentsNum, isSymbolInsertedToTable, countLines = 1;
    
    FILE *fp;

    SymbolNode *symbolTable = (SymbolNode *) malloc(sizeof(SymbolNode));
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
        if ((lineArgumentsNum == 0) || *line == ";") continue;
        
        /* first field is a label - instruction / command is the second field */
        if (strcmp(&firstLineField[strlen(firstLineField) - 1], ":") == 0)
        {
            strncpy(symbolName, firstLineField, strlen(firstLineField) - 2);
            isSymbol = TRUE;
            lineArgumentsNum = sscanf(line, "%s", firstLineField, action);
        }
        /* no symbolName - instruction / command is the first field */
        else strcpy(action, firstLineField);

        /* is current instruction for data storage */
        if (strcmp(action, ".data") == 0|| strcmp(action, ".string") == 0)
        {
            if (isSymbol) insertSymbolToTable(symbolTable, symbolName, IC, DATA);
            DC = encodeData(isSymbol? *(line + strlen(symbolName)): line, dataImage, DC);
        }
        /* if current instruction of type extern inserts it to the symbol table */
        else if (strcmp(action, ".extern") == 0)
        {
            if (isSymbol) insertSymbolToTable(symbolTable, firstLineField, 0, EXTERNAL);
            continue;
        }

        /* if current instruction of type entry it would be handled in the second iteration */
        else if (strcmp(action, ".entry") == 0)
        {
            if (isSymbol) insertSymbolToTable(symbolTable, firstLineField, 0, ENTRY);
            else continue;
        }

        /* line is a command */
        else
        {
            if (isSymbol)
            {
                isSymbolInsertedToTable = insertSymbolToTable(symbolTable, firstLineField, IC, CODE);
                printf("insert command to symbol table result : %d\n", isSymbolInsertedToTable);
                if (!isSymbolInsertedToTable)
                    continue;
            }
            printf("symbol inserted to table\n");
            if (!isValidCommand(action))
            {
                printf("Invalid action name - got %s\n", action);
                isError = TRUE;
                continue;
            }
            printf("computing command words\n");
            L = computeCommandWords(line, IC, isSymbol, action, instructionDataHead);
            IC += L;
            if (IC > MEMORY_SIZE)
            {
                printf("Memory exeeds available memory!");
                isError = TRUE;
            }
        }
    }

    printf("cheking errors\n");
    if (isError == TRUE)
    {
        printf("Errors occured in the first assembler iteration, program stops!");
        exit(1);
    }

    updateDataSymbols(symbolTable, IC);

    IterationsData *response = (IterationsData *) malloc(sizeof(IterationsData));
    response->symbolTable = symbolTable;
    response->instructionData = instructionDataHead;
    response->dataImage = dataImage;
    
    printData(response);
    
    return response;

}

void printData(IterationsData *response)
{
    SymbolNode *symPtr = response->symbolTable;
    InstructionData *insPtr = response->instructionData;

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

int computeCommandWords(char line[], int ic, int isSymbol, char command[], InstructionData *instructionDataHead)
{
    /* stop & rts get no operands */
    printf("compute command words for %s\n", command);
    //printf("condition is: %d\n", strcmp(command, "rts") == 0|| strcmp(command, "stop") == 0);
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
    char *sourceOperand;
    operandData *sourceOperandData;
    if (commas == 1)
    {
        if (allowedOperands != 2)
        {
            printf("Command %s must get %d operands but got 2 for line:\n%s\n!!", command, allowedOperands, line);
        }
        
        sourceOperand = strtok(line + strlen(command) + 1, ",");
        sourceOperandData = analyseOperand(line, command, sourceOperand, SOURCE, &words);
    }
    
    char *desOperand;
    uint8_t desAddressMethod;

    /* Zero operands command - if a legal non-operands command returns 1, otherwise return -1 (error) */
    desOperand = commas == 1? strtok(NULL, ",") : strtok(line + strlen(command) + 1, ",");
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
    operandData *destinationOperandData = analyseOperand(line, command, desOperand, DESTINATION, &words);

    printf("Source operand: %s %d %d\n", sourceOperandData->label, sourceOperandData->registerNum, sourceOperandData->value);

    saveInstructionData(command, ic, words, sourceOperandData != NULL? sourceOperand: 0, destinationOperandData, instructionDataHead);

    return words;
}

/* return the operand data - register / value / label and increase the words count accordingly */
operandData *analyseOperand(char line[], char command[], char *operand, OperandType operandType, int *words)
{
    operandData *opData;
    sscanf(operand, "%s", operand);
    printf("Operand is: %s\n", operand);

    opData = addressingMethodByOperand(operand);
    if (!isAllowedAddressingMethodByCommand(command, opData->addressingMethod, operandType))
    {
        printf("Invalid addressing method for source operand - line %s!", line);
        opData->addressingMethod = 0;
        return opData;
    }
    
    printf("words before for %s - %d\n", command, *words);
    int wordsByAdressing = getWordsNumByAdressMethod(opData->addressingMethod);
    printf("words number is: %d\n", wordsByAdressing);
    *words += wordsByAdressing;
    printf("words for %s - %d\n", command, *words);
    return opData;
}

/* update data symbols by adding IC + 100 to every address */
void *updateDataSymbols(SymbolNode *symbolTable, int finalIC)
{
    SymbolNode *ptr = symbolTable;
    while (ptr != NULL)
    {
        if (ptr->attribute1 == DATA || ptr->attribute2 == DATA) ptr->value += finalIC;
        ptr = ptr->next;
    }
}


int main(int argc, char const *argv[])
{
    IterationsData *data;// = (IterationsData *) malloc(sizeof(IterationsData));
    printf("%s\n", argv[1]);
    printf("%s\n", argv[1]);
    data = startAssemblerFirstIteration(argv[1]);
    // while (--argc > 0)
    // {
    //     printf("%s\n", *++argv);
    //     data = startAssemblerFirstIteration(argv[1]);
    // }
    return 0;
}
