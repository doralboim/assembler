#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"
#include "instructions.h"

InstructionData *saveInstructionData(char *command, int ic, int words, operandData *sourceOperand, operandData *destinatinoOperand, InstructionData *instructionsDataHead)
{
    printf("saving instruction data for command %s\n", command);
    InstructionData *newNode = (InstructionData *) malloc(sizeof(InstructionData));
    //strcpy(newNode->command, command);
    newNode->IC = ic;
    newNode->words = words;

    //newNode->sourceOperand = sourceOperand;
    //newNode->destinationOperand = destinatinoOperand;
    
    //newNode->next = NULL;

    printf("instruction head is: %s, IC->%s\n", instructionsDataHead->command, instructionsDataHead->IC);
    if (!instructionsDataHead->IC)
    {
        printf("instruction %s is null!!!!!!!!!\n", instructionsDataHead->command);
        instructionsDataHead = (InstructionData *) malloc(sizeof(InstructionData));
        instructionsDataHead = newNode;
    }
    else
    {
        InstructionData *ptr = instructionsDataHead;
        while (ptr->next != NULL) ptr++;
        ptr->next = newNode;
    }

    printf("completing saving instruction data for %s\n", instructionsDataHead->command);
    return instructionsDataHead;
    
}


// int main(int argc, char const *argv[])
// {
//     InstructionData *instructionDataHead = (InstructionData *) malloc(sizeof(InstructionData));
//     operandData *sourceOperand= (operandData *) malloc(sizeof(operandData));
//     sourceOperand->addressingMethod = 1;
//     sourceOperand->registerNum = 2;
//     operandData *desOperand= (operandData *) malloc(sizeof(operandData));
//     instructionDataHead = saveInstructionData("lea", 100, 3, sourceOperand, desOperand, instructionDataHead);
//     printf("instruction data: commnd: %s", instructionDataHead->command);
//     return 0;
// }
