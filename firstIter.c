#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "assembler.h"
#include "constants.h"
#include "dataHandler.h"
#include "utils.h"
#include "instructions.h"

#define IsActionExtern (strcmp(action, ".extern") == 0)
#define IsActionEntry (strcmp(action, ".entry") == 0)

int computeCommandWords(char line[], int ic, int isSymbol, char command[], InstructionData **instructionDataHead);
void updateDataSymbols(SymbolNode **symbolTable, int finalIC);
operandData *analyseOperand(char line[], char command[], char *operand, OperandType operandType, int *words);
void printData(IterationsData *response);

IterationsData *startAssemblerFirstIteration(char *fileName)
{
    char line[MAX_LINE_SIZE + 1], firstLineField[MAX_LINE_SIZE], action[MAX_LINE_SIZE], *symbolName;
    int *dataImage;
    int isError = FALSE, isSymbol;
    int IC = 0, DC = 0, L;
    int lineArgumentsNum, isSymbolInsertedToTable, countLines = 1;
    
    FILE *fp;
    SymbolNode *symbolTableHead;
    InstructionData *instructionDataHead;
    IterationsData *response;

    symbolName = (char *) malloc(MAX_SYMBOL_NAME_LENGTH);
    dataImage = (int *) malloc(MEMORY_SIZE * sizeof(int));
    symbolTableHead = NULL;
    instructionDataHead = NULL;

    if ((fp = fopen(fileName, "r")) == NULL)
    {
        fprintf(stderr, "Could not open file %s!", fileName);
        exit(0);
    }

    while(fgets(line, MAX_LINE_SIZE + 1, fp) != NULL)
    {
        printf("\n\n\n%d - %s\n", countLines++, line);
        memset(firstLineField, '\0', MAX_LINE_SIZE);
        lineArgumentsNum = sscanf(line, "%s", firstLineField);

        isSymbol = FALSE;

        /* if line is a space or comment - it is ignored */
        if ((lineArgumentsNum == 0) || *line == ';') continue;
        
        /* first field is a label - instruction / command is the second field */
        if (strcmp(&firstLineField[strlen(firstLineField) - 1], ":") == 0)
        {
            memset(symbolName, '\0', strlen(firstLineField));
            strncpy(symbolName, firstLineField, strlen(firstLineField) - 1);
            isSymbol = TRUE;
            lineArgumentsNum = sscanf(line, "%s%s", firstLineField, action);
        }
        /* no symbolName - instruction / command is the first field */
        else strcpy(action, firstLineField);

        /* Current instruction is for data / string storage */
        if (strcmp(action, ".data") == 0|| strcmp(action, ".string") == 0)
        {
            if (isSymbol) insertSymbolToTable(&symbolTableHead, symbolName, DC, DATA);
            DC = encodeData(line, dataImage, DC);
        }
        /* if current instruction of type extern or entry inserts it to the symbol table */
        else if (IsActionExtern || IsActionEntry)
        {
            
            sscanf(line, "%s%s", firstLineField, symbolName);
            insertSymbolToTable(&symbolTableHead, symbolName, 0, IsActionExtern ? EXTERNAL: ENTRY);
            continue;
        }

        /* Current is a command line */
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
                isSymbolInsertedToTable = insertSymbolToTable(&symbolTableHead, symbolName, IC, CODE);
                if (!isSymbolInsertedToTable)
                {
                    isError = TRUE;
                    continue;
                }
            }

            L = computeCommandWords(line, IC, isSymbol, action, &instructionDataHead);
            IC += L;
            
            /* verify that IC does not exceed the memory size  */
            if (IC > MEMORY_SIZE)
            {
                fprintf(stderr, "Memory exeeds available memory!");
                isError = TRUE;
            }
        }
    }

    if (isError == TRUE)
    {
        fprintf(stderr, "Errors occured in the first assembler iteration, program stops!");
        exit(1);
    }

    /* Reallocating data to follow code instructions */
    updateDataSymbols(&symbolTableHead, IC);

    response = (IterationsData *) malloc(sizeof(IterationsData));

    response->symbolTable = symbolTableHead;
    response->instructionData = instructionDataHead;
    response->dataImage = dataImage;
    response->IC = IC;
    response->DC = DC;
    
    printData(response);
    fclose(fp);
    return response;

}

void printData(IterationsData *response)
{
    SymbolNode *symPtr = response->symbolTable;
    InstructionData *insPtr = response->instructionData;
    int i;
    for (i=0; i < response->DC; i++) printf("data image on place %d is: %d\n", i, (response->dataImage)[i]);

    while (symPtr != NULL)
    {
        printf("symbol: %s %d %d\n", symPtr->symbolName, symPtr->value, symPtr->attribute1);
        symPtr = symPtr->next;
    }

    while (insPtr != NULL)
    {
        printf("instructions: %s %d %d\n", insPtr->command, insPtr->IC, insPtr->words);
        insPtr = insPtr->next;
    }
}

/* The core function of the first iteration, which analyses the given line, parses the operands and compute the machine words */
int computeCommandWords(char line[], int ic, int isSymbol, char command[], InstructionData **instructionDataHead)
{
    
    int words, commas, allowedOperands;
    char *lineWithoutSymbol, *sourceOperand, *desOperand;
    
    operandData *sourceOperandData, *destinationOperandData;

    /* stop & rts get no operands */
    if (strcmp(command, "rts") == 0 || strcmp(command, "stop") == 0)
    {
        saveInstructionData(command, ic, 1, NULL, NULL, instructionDataHead);
        return 1;
    }
    
    lineWithoutSymbol = strstr(line, command);
    commas = getNumberOfCommas(lineWithoutSymbol);
    words = 2;
    
    /* more than 1 comma means more than 2 operands which is iillegal */
    if (commas > 1)
    {
        fprintf(stderr, "Illegal number of operands, expected max of 2 and got %d\n", commas - 1);
        return -1;
    }

    /* obtain the allowed words of the given command and utilizes it for validation along the function */
    allowedOperands = getNumberOfAllowedOperandsByCommand(command);
    sourceOperandData = NULL;

    /* 1 comma = 2 operands are introduced */
    if (commas == 1)
    {
        if (allowedOperands != 2)
        {
            fprintf(stderr, "Command %s must get %d operands but got 2 for lineWithoutSymbol:\n%s\n!!", command, allowedOperands, lineWithoutSymbol);
            return -1;            
        }
        
        sourceOperand = strtok(lineWithoutSymbol + strlen(command) + 1, ",");
        sourceOperandData = analyseOperand(lineWithoutSymbol, command, sourceOperand, SOURCE, &words);
    }

    /* Obtain the destination operand according to the existence of a source operand */
    desOperand = commas == 1? strtok(NULL, ",") : strtok(lineWithoutSymbol + strlen(command) + 1, ",");
    sscanf(desOperand, "%s", desOperand);

    if (desOperand == NULL)
    {
        if (allowedOperands != 0)
        {
            fprintf(stderr, "Command %s must get %d operands but got 0 for lineWithoutSymbol:\n%s!!\n", command, allowedOperands, lineWithoutSymbol);
            return -1;
        }
        return 1;
    }

    /* check if actions is allowed with 1 operand */
    if (commas == 0 && allowedOperands != 1)
    {
        fprintf(stderr, "Command %s must get %d operands but got 1 for lineWithoutSymbol:\n%s!!\n", command, allowedOperands, lineWithoutSymbol);
        return -1;
    }

    destinationOperandData = analyseOperand(lineWithoutSymbol, command, desOperand, DESTINATION, &words);
    if (destinationOperandData == NULL) return -1;
    saveInstructionData(command, ic, words, sourceOperandData, destinationOperandData, instructionDataHead);

    return words;
}

/* return the operand data - register / value / label and increase the words count accordingly */
operandData *analyseOperand(char line[], char command[], char *operand, OperandType operandType, int *words)
{
    int wordsByAdressing;
    operandData *opData;
    
    sscanf(operand, "%s", operand);

    opData = addressingMethodByOperand(operand);
    if (!isAllowedAddressingMethodByCommand(command, opData->addressingMethod, operandType))
    {
        fprintf(stderr, "Invalid addressing method for source operand on line: %s!", line);
        opData->addressingMethod = 0;
        return NULL;
    }
    
    wordsByAdressing = getWordsNumByAdressMethod(opData->addressingMethod);
    *words += wordsByAdressing;
    opData->numOfWords = wordsByAdressing;
    return opData;
}

/* update data symbols by adding IC + 100 to every address */
void updateDataSymbols(SymbolNode **symbolTable, int finalIC)
{
    SymbolNode *ptr = *symbolTable;
    while (ptr != NULL)
    {
        if (ptr->attribute1 == DATA) ptr->value += finalIC;
        ptr = ptr->next;
    }
}
