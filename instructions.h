#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

typedef struct struct_action {
    char name[10];
    uint8_t opcode;
    uint8_t funct;
    unsigned int sourceAdressingMethods:4;
    unsigned int targetAdressingMethods:4;
} CommandNode;

#endif