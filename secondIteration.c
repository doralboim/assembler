#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#include "assembler.h"
#include "constants.h"
#include "instructions.h"
#include "encoder.h"

long *encodeOperand(operandData *operand, SymbolNode *symbolTable, OperandType opType);
SymbolNode *getLabelNode(SymbolNode *symbolTable, char *label);

/* encoding program instructions from the instructions data object, utilizing the symbol table */
long *startSecondIteration(IterationsData *firstIterData)
{
    InstructionData *instructionsPtr = firstIterData->instructionData;
    InstructionData *temp;
    short countEncoded;
    int errors = 0;

    long *instructionImage = (long *) malloc((firstIterData->IC) * sizeof(long));
    long *sourceOperandEncoding, *destinationOperandEncoding;

    while (instructionsPtr != NULL)
    {
        // printf("working on %s - num words: %d IC = %d\n", instructionsPtr->command, instructionsPtr->words, instructionsPtr->IC);

        *(instructionImage + instructionsPtr->IC) = encodeFirstWord(instructionsPtr->command);
        if (*(instructionImage + instructionsPtr->IC) == -1) errors++;
        
        else
        {
            if (instructionsPtr->words >= 2)
            {
                countEncoded = 1;
                *(instructionImage + instructionsPtr->IC + 1) = encodeSecWord(instructionsPtr->command, instructionsPtr->sourceOperand, instructionsPtr->destinationOperand);
                countEncoded++;            
            
                sourceOperandEncoding = instructionsPtr->sourceOperand != NULL && instructionsPtr->sourceOperand->numOfWords > 0 ? \
                                        encodeOperand(instructionsPtr->sourceOperand, firstIterData->symbolTable, SOURCE) : NULL;
                destinationOperandEncoding = instructionsPtr->destinationOperand != NULL && instructionsPtr->destinationOperand->numOfWords > 0 ? \
                                            encodeOperand(instructionsPtr->destinationOperand, firstIterData->symbolTable, DESTINATION) : NULL;
                if ((sourceOperandEncoding != NULL && *sourceOperandEncoding == -1) ||
                    (destinationOperandEncoding != NULL && *destinationOperandEncoding == -1)) errors++;

                else
                {
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
            }
        }

        temp = instructionsPtr;
        instructionsPtr = instructionsPtr->next;
        free(temp);
    }

    if (errors > 0)
    {
        fprintf(stderr, "Got %d errors on the second iteration, program stops\n", errors);
        exit(1);
    }

    return instructionImage;
}

/* encode operand according to the operand data (label/register/value) retrieved in the first iteration */
long *encodeOperand(operandData *operand, SymbolNode *symbolTable, OperandType opType)
{
    SymbolNode *labelNode = NULL;
    long *encoding;

    switch (operand->addressingMethod)
    {
        /* Immediate addressing - encoding the operand based upon its value */
        case IMMEDIATE_ADDRESING:
            encoding = encodeImmediateAdressing(operand->value);
            break;

        /* labeled operands - encoding operand base and offset (in case of index addressing the operand register would be encoded in the second word) */
        case DIRECT_ADDRESING: case INDEX_ADDRESSING:
            labelNode = getLabelNode(symbolTable, operand->label);
            if (labelNode == NULL)
            {
                fprintf(stderr, "Operand has invalid label - %s\n", operand->label);
                encoding = (long *) malloc(sizeof(long));
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
    SymbolNode *ptr = symbolTable;
    while (ptr != NULL)
    {
        if (strcmp(ptr->symbolName, label) == 0 && ptr->attribute1 != EMPTY) return ptr;
        ptr = ptr->next;
    }
    return NULL;
}
