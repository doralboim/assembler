#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#include "assembler.h"
#include "constants.h"
#include "encoder.h"


/* encoding program instructions from the instructions data object, utilizing the symbol table */
void encodeAllInstructions(char *fileName, IterationsData *firstIterData)
{
    SymbolNode *symbolTablePtr = firstIterData->symbolTable;
    InstructionData *instructionsPtr = firstIterData->instructionData;
    int *instructionImage, isError = FALSE;

    int firstWordEncoding, secWordEncoding, *sourceOperandEncoding, *destinationOperandEncoding;
    /* track how many words were discarded due to illegal labels and reduce IC accordingly */
    int accumulatedErrors;

    while (instructionsPtr != NULL)
    {
        if (instructionsPtr->words >= 5)
        {
            *sourceOperandEncoding = encodeOperand(instructionsPtr->sourceOperand, symbolTablePtr, SOURCE);
            if (*sourceOperandEncoding == -1)
            {
                isError = TRUE;
                continue;
            }
            *(instructionImage + instructionsPtr->IC + 4) = *sourceOperandEncoding;
            if (instructionsPtr->words == 6) *(instructionImage + instructionsPtr->IC + 5) = sourceOperandEncoding[1];
        }

        if (instructionsPtr->words >= 3)
        {
            *destinationOperandEncoding = encodeOperand(instructionsPtr->destinationOperand, symbolTablePtr, DESTINATION);
            if (*destinationOperandEncoding == -1)
            {
                isError = TRUE;
                continue;
            }
            *(instructionImage + instructionsPtr->IC + 2) = *destinationOperandEncoding;
            if (instructionsPtr->words == 4) *(instructionImage + instructionsPtr->IC + 3) = destinationOperandEncoding[1];
        }

        if (instructionsPtr->words >= 2) *(instructionImage + instructionsPtr->IC + 1) = encodeSecWord(instructionsPtr->command,
                                                                         instructionsPtr->sourceOperand->registerNum,
                                                                         instructionsPtr->sourceOperand->addressingMethod,
                                                                         instructionsPtr->destinationOperand->registerNum,
                                                                         instructionsPtr->destinationOperand->addressingMethod);
        *(instructionImage + instructionsPtr->IC) = encodeFirstWord(instructionsPtr->command);
        instructionsPtr++;
    }
}

/* encode operand according to the operand data retrieved in the first iteration */
int *encodeOperand(operandData *operand, SymbolNode *symbolTable, OperandType opType)
{
    switch (operand->addressingMethod)
    {
    case IMMEDIATE_ADDRESING:
    {
        int *encoding = encodeImmediateAdressing(operand->value);
        return encoding;
    }

    case DIRECT_ADDRESING: case INDEX_ADDRESSING:
        SymbolNode *labelNode = getLabelNode(symbolTable, operand->label);
        if (labelNode == NULL)
        {
            printf("Operand has invalid label %s!", operand->label);
            return -1;
        }
        return encodeDirectAdressing(labelNode);
    }

    return 0;
}

/* get lable node from the symbol table */
SymbolNode *getLabelNode(SymbolNode *symbolTable, char *label)
{   
    SymbolNode *ptr = symbolTable;
    while (ptr != NULL)
    {
        if (strcmp(ptr->symbolName, label) == 0) return ptr;
        ptr++;
    }
    return NULL;
}