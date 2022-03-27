#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "assembler.h"
#include "constants.h"
#include "encoder.h"
#include "utils.h"
#include "instructions.h"

#define IsActionExtern (strcmp(action, ".extern") == 0)
#define IsActionEntry (strcmp(action, ".entry") == 0)

int computeCommandWords(char line[], int ic, int isSymbol, char command[], InstructionData **instructionDataHead, int lineNum);
void updateDataSymbols(SymbolNode **symbolTable, int finalIC);
operandData *analyseOperand(char line[], char command[], char *operand, OperandType operandType, int *words, int lineNum);
void printData(IterationsData *response);

IterationsData *startAssemblerFirstIteration(char *fileName)
{
    char line[MAX_LINE_SIZE + 1], firstLineField[MAX_LINE_SIZE], action[MAX_LINE_SIZE], *symbolName;
    long *dataImage;
    int errors = 0, isValidResponse, isSymbol;
    int IC = 0, DC = 0, L, lineNum = 1;
    int lineArgumentsNum;
    
    FILE *fp;
    SymbolNode *symbolTableHead;
    InstructionData *instructionDataHead;
    IterationsData *response;

    symbolName = (char *) malloc(MAX_SYMBOL_NAME_LENGTH);
    dataImage = (long *) malloc(MEMORY_SIZE * sizeof(long));
    symbolTableHead = NULL;
    instructionDataHead = NULL;

    if ((fp = fopen(fileName, "r")) == NULL)
    {
        fprintf(stderr, "Could not open file %s\n", fileName);
        exit(0);
    }

    while(fgets(line, MAX_LINE_SIZE + 1, fp) != NULL)
    {
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

            if (lineArgumentsNum == 1)
            {
                fprintf(stderr, "line %s\n", line);
                fprintf(stderr, "Label %s is defined without instructions\n", symbolName);
                errors++;
                continue;
            }
        }
        /* no symbolName - instruction / command is the first field */
        else strcpy(action, firstLineField);

        /* Current instruction is for data / string storage */
        if (strcmp(action, DATA_INSTRUCTION) == 0|| strcmp(action, STRING_INSTRUCTION) == 0)
        {
            if (isSymbol)
            {
                isValidResponse = insertSymbolToTable(&symbolTableHead, symbolName, DC, DATA);
                if (!isValidResponse)
                {
                    errors++;
                    continue;
                }

            }
            DC = encodeData(line, dataImage, DC);
            if (DC == -1)
            {
                errors++;
                continue;
            }
        }
        /* if current instruction of type extern or entry inserts it to the symbol table */
        else if (IsActionExtern || IsActionEntry)
        {
            
            lineArgumentsNum = sscanf(line, "%s%s", firstLineField, symbolName);
            if (lineArgumentsNum == 1)
            {
                fprintf(stderr, "line %s\n", line);
                fprintf(stderr, "Invalid declaration %s without a given label", action);
                errors++;
                continue;
            }
            isValidResponse = insertSymbolToTable(&symbolTableHead, symbolName, 0, IsActionExtern ? EXTERNAL: ENTRY);
            if (!isValidResponse) errors++;
            continue;
        }

        /* Current is a command line */
        else
        {
            if (!isValidCommand(action))
            {
                fprintf(stderr, "line %s\n", line);
                fprintf(stderr, "\tInvalid action name - got %s\n", action);
                errors++;
                continue;
            }
            
            if (isSymbol)
            {
                isValidResponse = insertSymbolToTable(&symbolTableHead, symbolName, IC, CODE);
                if (!isValidResponse)
                {
                    errors++;
                    continue;
                }
            }

            L = computeCommandWords(line, IC, isSymbol, action, &instructionDataHead, lineNum);
            if (L == -1)
            {
                errors++;
                continue;
            }
            IC += L;
            
            /* verify that IC does not exceed the memory size  */
            if (IC > MEMORY_SIZE)
            {
                fprintf(stderr, "Memory required exeeds available memory.\n");
                fprintf(stderr, "\tMemory available is %d but required is %d\n", MEMORY_SIZE, IC);
                fprintf(stderr, "Occured for line %s\n", line);
                errors++;
            }
        }

        lineNum++;
    }

    if (errors > 0)
    {
        fprintf(stderr, "%d errors occured in the first assembler iteration, program stops!", errors);
        return NULL;
    }

    /* Reallocating data to follow code instructions */
    updateDataSymbols(&symbolTableHead, IC);

    response = (IterationsData *) malloc(sizeof(IterationsData));

    response->symbolTable = symbolTableHead;
    response->instructionData = instructionDataHead;
    response->dataImage = dataImage;
    response->IC = IC;
    response->DC = DC;
    
    fclose(fp);
    return response;

}

/* The core function of the first iteration, which analyses the given line, parses the operands and compute the machine words */
int computeCommandWords(char line[], int ic, int isSymbol, char command[], InstructionData **instructionDataHead, int lineNum)
{
    
    int words, commas, allowedOperands;
    char *lineWithoutSymbol, *sourceOperand, *desOperand;
    
    operandData *sourceOperandData, *destinationOperandData;

    /* stop & rts get no operands */
    if (strcmp(command, "rts") == 0 || strcmp(command, "stop") == 0)
    {
        if (!saveInstructionData(command, ic, 1, NULL, NULL, instructionDataHead, lineNum)) return -1;
        return 1;
    }
    
    lineWithoutSymbol = strstr(line, command);
    commas = getNumberOfCommas(lineWithoutSymbol);
    words = 2;
    
    /* more than 1 comma means more than 2 operands which is iillegal */
    if (commas > 1)
    {
        fprintf(stderr, "Error on line number %d: %s:\n", lineNum, line);
        fprintf(stderr, "\tIllegal number of operands, expected max of 2 and got %d\n", commas + 1);
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
            fprintf(stderr, "Error on line number %d: %s:\n", lineNum, line);
            fprintf(stderr, "\tCommand %s must get %d operands but got 2\n", command, allowedOperands);
            return -1;            
        }
        
        sourceOperand = strtok(lineWithoutSymbol + strlen(command) + 1, ",");
        sourceOperandData = analyseOperand(lineWithoutSymbol, command, sourceOperand, SOURCE, &words, lineNum);
        if (sourceOperandData == NULL) return -1;
    }

    /* Obtain the destination operand according to the existence of a source operand */
    desOperand = commas == 1? strtok(NULL, ",") : strtok(lineWithoutSymbol + strlen(command) + 1, ",");
    if (desOperand == NULL)
    {
        fprintf(stderr, "Error on line number %d: %s:\n", lineNum, line);
        fprintf(stderr, "\tCommand %s must get %d operands but got nothing\n", command, allowedOperands);
        return -1;
    }
    sscanf(desOperand, "%s", desOperand);

    
    if (commas == 0 && allowedOperands != 1)
    {
        fprintf(stderr, "Error on line number %d: %s:\n", lineNum, line);
        fprintf(stderr, "\tCommand %s must get %d operands but got 1\n", command, allowedOperands);
        return -1;
    }

    destinationOperandData = analyseOperand(lineWithoutSymbol, command, desOperand, DESTINATION, &words, lineNum);
    if (destinationOperandData == NULL) return -1;
    if (!saveInstructionData(command, ic, words, sourceOperandData, destinationOperandData, instructionDataHead, lineNum)) return -1;    

    return words;
}

/* return the operand data - register / value / label and increase the words count accordingly */
operandData *analyseOperand(char line[], char command[], char *operand, OperandType operandType, int *words, int lineNum)
{
    int wordsByAdressing;
    operandData *opData;
    
    sscanf(operand, "%s", operand);

    opData = addressingMethodByOperand(operand);
    /* candidate for being NULL - for immediate addressing where the operand value exceeds short range */
    if (opData == NULL) return NULL; 

    if (!isAllowedAddressingMethodByCommand(command, opData->addressingMethod, operandType))
    {
        fprintf(stderr, "Error on line number %d: %s:\n", lineNum, line);
        fprintf(stderr, "\tInvalid addressing method for source operand on line: %s\n", line);
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
