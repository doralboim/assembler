#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MEMORY_SIZE 8192
#define MEMORY_PARTITION 20
#define MAX_LINE_SIZE 80
#define MAX_SYMBOL_NAME_LENGTH 32
#define TRUE 1
#define FALSE 0
#define REGISTERS_AMOUNT 16
#define COMMANDS_AMOUNT 16
#define BEGIN_CODE_LOAD 100
#define MAX_ENCODED_WORDS_PER_COMMAND 6

#define ADDRESS_METHOD_AMOUNT 4
#define IMMEDIATE_ADDRESING 0x01
#define DIRECT_ADDRESING 0x02
#define INDEX_ADDRESSING 0x04
#define REGISTER_DIRECT_ADDRESSING 0x08
#define ALL_ADDRESSING_METHODS IMMEDIATE_ADDRESING | DIRECT_ADDRESING | INDEX_ADDRESSING | REGISTER_DIRECT_ADDRESSING
#define DIRECT_INDEX_REGISTER_ADDRESSING DIRECT_ADDRESING | INDEX_ADDRESSING | REGISTER_DIRECT_ADDRESSING

#define EXTERNAL_ENCODING_TYPE 0x01
#define RELOCATABLE_ENCODING_TYPE 0x02
#define ABSOLUTE_ENCODING_TYPE 0x04

#include <stdlib.h>

typedef struct struct_action {
    char name[10];
    uint8_t opcode;
    uint8_t funct;
    unsigned int sourceAdressingMethods:4;
    unsigned int targetAdressingMethods:4;
} CommandNode;

typedef struct address_method_words{
    uint8_t method;
    uint8_t numOfWords;
    int methodNum;
} AddressMethodWords;

typedef enum {
    SOURCE, DESTINATION
} OperandType;

typedef enum {
    IMMEDIATE, DIRECT, INDEX, REGISTER_DIRECT
} AddressingMethods;

extern const AddressMethodWords *wordsPerAddressMethods();
extern CommandNode *machine_commands();

#endif