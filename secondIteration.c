#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#include "assembler.h"
#include "constants.h"
#include "instructions.h"
#include "encoder.h"

int *encodeOperand(operandData *operand, SymbolNode *symbolTable, OperandType opType);
SymbolNode *getLabelNode(SymbolNode *symbolTable, char *label);

/* encoding program instructions from the instructions data object, utilizing the symbol table */
int *startSecondIteration(IterationsData *firstIterData)
{
    printf("\n\nDC = %d  IC = %d\n", firstIterData->DC, firstIterData->IC);
    printf("started second iteration\n");
    printf("sec line in sec iter!\n");
    //SymbolNode firstIterData->symbolTable = firstIterData->symbolTable;
    InstructionData *instructionsPtr = firstIterData->instructionData;
    int isError = FALSE;

    int *instructionImage = (int *) malloc((firstIterData->IC) * sizeof(int));
    int firstWordEncoding, secWordEncoding, *sourceOperandEncoding, *destinationOperandEncoding;
    /* track how many words were discarded due to illegal labels and reduce IC accordingly */
    int accumulatedErrors;

    while (instructionsPtr != NULL)
    {
        printf("working on %s - num words: %d IC = %d\n", instructionsPtr->command, instructionsPtr->words, instructionsPtr->IC);
        if (instructionsPtr->words >= 5)
        {
            printf("5 length words!\n");
            sourceOperandEncoding = encodeOperand(instructionsPtr->sourceOperand, firstIterData->symbolTable, SOURCE);
            if (*sourceOperandEncoding == -1)
            {
                printf("Error encoding source operand for %s\n", instructionsPtr->command);
                isError = TRUE;
                instructionsPtr = instructionsPtr->next;
                continue;
            }
            *(instructionImage + instructionsPtr->IC + 4) = *sourceOperandEncoding;
            if (instructionsPtr->words == 6) *(instructionImage + instructionsPtr->IC + 5) = sourceOperandEncoding[1];
        }

        if (instructionsPtr->words >= 3)
        {
            printf("3 length words!\n");
            destinationOperandEncoding = encodeOperand(instructionsPtr->destinationOperand, firstIterData->symbolTable, DESTINATION);
            printf("encoded destination operand\n");
            /* // if (*destinationOperandEncoding == -1)
            // {
            //     printf("Error encoding target operand for %s\n", instructionsPtr->command);
            //     isError = TRUE;
            //     instructionsPtr = instructionsPtr->next;
            //     continue;
            // } */
            
            *(instructionImage + instructionsPtr->IC + 2) = *destinationOperandEncoding;
            
            if (instructionsPtr->words == 4) *(instructionImage + instructionsPtr->IC + 3) = destinationOperandEncoding[1];
            
        }

        if (instructionsPtr->words >= 2)
        {
            printf("encoding second word!\n");
            *(instructionImage + instructionsPtr->IC + 1) = encodeSecWord(instructionsPtr->command, instructionsPtr->sourceOperand, instructionsPtr->destinationOperand);
        }
        printf("encoding first word\n");
        *(instructionImage + instructionsPtr->IC) = encodeFirstWord(instructionsPtr->command);
        instructionsPtr = instructionsPtr->next;
    }

    printf("\n\nDC = %d  IC = %d\n", firstIterData->DC, firstIterData->IC);
    return instructionImage;
}

/* encode operand according to the operand data retrieved in the first iteration */
int *encodeOperand(operandData *operand, SymbolNode *symbolTable, OperandType opType)
{
    printf("encoding operand!!\n");
    SymbolNode *labelNode = NULL;
    int *encoding;
    switch (operand->addressingMethod)
    {
        case IMMEDIATE_ADDRESING:
            printf("immediate addressing!\n");
            encoding = (int *) malloc(sizeof(int));
            *encoding = encodeImmediateAdressing(operand->value);
            return encoding;

        case DIRECT_ADDRESING: case INDEX_ADDRESSING:
            printf("direct addressing!\n");
            labelNode = getLabelNode(symbolTable, operand->label);
            printf("got label for %s\n", operand->label);
            if (labelNode == NULL)
            {
                fprintf(stderr, "Operand has invalid label %s!", operand->label);
                return -1;
            }
            encoding = encodeDirectAdressing(labelNode);
            printf("got result for %s\n", operand->label);
            return encoding;
    }

    return 0;
}

/* get lable node from the symbol table */
SymbolNode *getLabelNode(SymbolNode *symbolTable, char *label)
{   
    SymbolNode *ptr = symbolTable;
    while (ptr != NULL)
    {
        printf("label - %s\n", ptr->symbolName);
        if (strcmp(ptr->symbolName, label) == 0) return ptr;
        ptr = ptr->next;
    }
    return NULL;
}

