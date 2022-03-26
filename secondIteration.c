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
    InstructionData *instructionsPtr = firstIterData->instructionData;
    short isError = FALSE, countEncoded;

    int *instructionImage = (int *) malloc((firstIterData->IC) * sizeof(int));
    int firstWordEncoding, secWordEncoding, *sourceOperandEncoding, *destinationOperandEncoding;

    while (instructionsPtr != NULL)
    {
        printf("working on %s - num words: %d IC = %d\n", instructionsPtr->command, instructionsPtr->words, instructionsPtr->IC);

        *(instructionImage + instructionsPtr->IC) = encodeFirstWord(instructionsPtr->command);
        countEncoded = 1;
        if (instructionsPtr->words >= 2)
        {
            *(instructionImage + instructionsPtr->IC + 1) = encodeSecWord(instructionsPtr->command, instructionsPtr->sourceOperand, instructionsPtr->destinationOperand);
            countEncoded++;            
        
            sourceOperandEncoding = instructionsPtr->sourceOperand != NULL && instructionsPtr->sourceOperand->numOfWords > 0 ? \
                                    encodeOperand(instructionsPtr->sourceOperand, firstIterData->symbolTable, SOURCE) : NULL;
            destinationOperandEncoding = instructionsPtr->destinationOperand != NULL && instructionsPtr->destinationOperand->numOfWords > 0 ? \
                                         encodeOperand(instructionsPtr->destinationOperand, firstIterData->symbolTable, DESTINATION) : NULL;

            if (sourceOperandEncoding != NULL)
            {
                *(instructionImage + instructionsPtr->IC + countEncoded++) = *(sourceOperandEncoding);
                if (instructionsPtr->sourceOperand->numOfWords == 2) *(instructionImage + instructionsPtr->IC + countEncoded++) = *(sourceOperandEncoding + 1);
            }

            if (destinationOperandEncoding != NULL)
            {
                *(instructionImage + instructionsPtr->IC + countEncoded++) = *(destinationOperandEncoding);
                if (instructionsPtr->destinationOperand->numOfWords == 2) *(instructionImage + instructionsPtr->IC + countEncoded++) = *(destinationOperandEncoding + 1);
            }
        }
        instructionsPtr = instructionsPtr->next;
    }

    printf("\n\nDC = %d  IC = %d\n", firstIterData->DC, firstIterData->IC);
    return instructionImage;
}

/* encode operand according to the operand data (label/register/value) retrieved in the first iteration */
int *encodeOperand(operandData *operand, SymbolNode *symbolTable, OperandType opType)
{
    printf("statring encodign operand. label is %s\n", operand->label);
    SymbolNode *labelNode = NULL;
    int *encoding;
    // printf("encoding operand for label %d\n", operand->addressingMethod);
    switch (operand->addressingMethod)
    {
        /* Immediate addressing - encoding the operand based upon its value */
        case IMMEDIATE_ADDRESING:
            printf("immediate addressing\n");
            encoding = (int *) malloc(sizeof(int));
            *encoding = encodeImmediateAdressing(operand->value);
            break;

        /* labeled operands - encoding operand base and offset (in case of index addressing the operand register would be encoded in the second word) */
        case DIRECT_ADDRESING: case INDEX_ADDRESSING:
            printf("getting label\n");
            labelNode = getLabelNode(symbolTable, operand->label);
            if (labelNode == NULL)
            {
                fprintf(stderr, "Operand has invalid label %s!", operand->label);
                encoding = (int *) malloc(sizeof(int));
                *encoding = -1;
                break;
            }
            encoding = encodeDirectAdressing(labelNode);
            break;
    }
    
    return encoding;
}

/* Search in the symbol table for the compatible label node for the given label */
SymbolNode *getLabelNode(SymbolNode *symbolTable, char *label)
{   
    printf("getting label node for %s\n", label);
    SymbolNode *ptr = symbolTable;
    while (ptr != NULL)
    {
        if (strcmp(ptr->symbolName, label) == 0) return ptr;
        ptr = ptr->next;
    }
    return NULL;
}
