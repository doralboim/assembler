#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"
#include "constants.h"
#include "instructions.h"


InstructionNode *createInstruction(char *name, uint8_t opcode, uint8_t funct, uint8_t sourceAddressing, uint8_t targetAddressing)
{
    InstructionNode *newNode;
    newNode = (InstructionNode *) malloc(sizeof(InstructionNode));
    strcpy(newNode->name, name);
    newNode->funct = funct;
    newNode->opcode = opcode;
    newNode->sourceAdressingMethods = sourceAddressing;
    newNode->targetAdressingMethods = targetAddressing;

    return newNode;
}


InstructionNode **createAllInstructions()
 {
     InstructionNode **allInstructions = (InstructionData *) malloc(sizeof(InstructionData) * INSTRUCTIONS_AMOUNT);
     
     
     allInstructions[0] = createInstruction("mov", 0, 0, ALL_ADDRESSING_METHODS, DIRECT_INDEX_REGISTER_ADDRESSING);
     allInstructions[1] = createInstruction("cmp", 1, 0, ALL_ADDRESSING_METHODS, ALL_ADDRESSING_METHODS);
     allInstructions[2] = createInstruction("add", 2, 10, ALL_ADDRESSING_METHODS, DIRECT_INDEX_REGISTER_ADDRESSING);
     allInstructions[3] = createInstruction("sub", 2, 11, ALL_ADDRESSING_METHODS, DIRECT_INDEX_REGISTER_ADDRESSING);
     allInstructions[4] = createInstruction("lea", 4, 0, DIRECT_ADDRESING | INDEX_ADDRESSING, DIRECT_INDEX_REGISTER_ADDRESSING);
     allInstructions[5] = createInstruction("clr", 5, 10, 0, DIRECT_INDEX_REGISTER_ADDRESSING);
     allInstructions[6] = createInstruction("not", 5, 11, 0, DIRECT_INDEX_REGISTER_ADDRESSING);
     allInstructions[7] = createInstruction("inc", 5, 12, 0, DIRECT_INDEX_REGISTER_ADDRESSING);
     allInstructions[8] = createInstruction("dec", 5, 13, 0, DIRECT_INDEX_REGISTER_ADDRESSING);
     allInstructions[9] = createInstruction("jmp", 9, 10, 0, DIRECT_ADDRESING | INDEX_ADDRESSING);
     allInstructions[10] = createInstruction("bne", 9, 11, 0, DIRECT_ADDRESING | INDEX_ADDRESSING);
     allInstructions[11] = createInstruction("jsr", 9, 12, 0, DIRECT_ADDRESING | INDEX_ADDRESSING);
     allInstructions[12] = createInstruction("red", 12, 0, 0, DIRECT_INDEX_REGISTER_ADDRESSING);
     allInstructions[13] = createInstruction("prn", 13, 0, 0, ALL_ADDRESSING_METHODS);
     allInstructions[14] = createInstruction("rts", 14, 0, 0, 0);
     allInstructions[15] = createInstruction("stop", 15, 0, 0, 0);

     return allInstructions;
 }