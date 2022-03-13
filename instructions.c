#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"
#include "instructions.h"

void saveInstructionData(char *command, int ic, int words, operandData *sourceOperand, operandData *destinatinoOperand, InstructionData *instructionsDataHead)
{
    InstructionData *newNode = (InstructionData *) malloc(sizeof(InstructionData));
    strcpy(newNode->command, command);
    newNode->IC = ic;
    newNode->words = words;

    newNode->sourceOperand = sourceOperand;
    newNode->destinationOperand = destinatinoOperand;
    
    newNode->next = NULL;

    if (instructionsDataHead == NULL)
    {
        instructionsDataHead = (InstructionData *) malloc(sizeof(InstructionData));
        instructionsDataHead = newNode;
    }
    else
    {
        InstructionData *ptr = instructionsDataHead;
        while (ptr->next != NULL) ptr++;
        ptr->next = newNode;
    }

    printf("%s", instructionsDataHead->command);
    
}
