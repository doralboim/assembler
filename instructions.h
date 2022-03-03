#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

typedef struct struct_action {
    char name[10];
    uint8_t funct;
    uint8_t opcode;
    unsigned int sourceAdressingMethods:4;
    unsigned int targetAdressingMethods:4;
} ActionNode;

ActionNode *createInstruction(char *name, uint8_t opcode, uint8_t funct, uint8_t sourceAddressing, uint8_t targetAddressing);
ActionNode **createAllInstructions();

#endif