#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"
#include "instructions.h"

/* Add instruction to linked list to be used in the second batch */
void saveInstructionData(char *command, int ic, int words, operandData *sourceOperand, operandData *destinatinoOperand, InstructionData **instructionsHead)
{
    InstructionData *newNode; 
    newNode = (InstructionData *) malloc(sizeof(InstructionData));
    if (newNode == NULL) printf("Memory cannot be allocated for saving instruction data!");

    else {
        memset(newNode->command, '\0', strlen(command) + 1);
        strcpy(newNode->command, command);
        newNode->IC = ic;
        newNode->words = words;

        newNode->sourceOperand = sourceOperand;
        newNode->destinationOperand = destinatinoOperand;
        
        newNode->next = NULL;

        if ((*instructionsHead) == NULL)
        {
            *instructionsHead = newNode;
        }
        else
        {
            InstructionData *ptr = *instructionsHead;
            while (ptr->next != NULL) ptr++;
            ptr->next = newNode;
        }
    }
}
