#include "constants.h"

CommandNode *machine_commands()
{
    CommandNode commands[] = {
    {"mov", 0, 0, ALL_ADDRESSING_METHODS, DIRECT_INDEX_REGISTER_ADDRESSING},
    {"cmp", 1, 0, ALL_ADDRESSING_METHODS, ALL_ADDRESSING_METHODS},
    {"add", 2, 10, ALL_ADDRESSING_METHODS, DIRECT_INDEX_REGISTER_ADDRESSING},
    {"sub", 2, 11, ALL_ADDRESSING_METHODS, DIRECT_INDEX_REGISTER_ADDRESSING},
    {"lea", 4, 0, DIRECT_ADDRESING | INDEX_ADDRESSING, DIRECT_INDEX_REGISTER_ADDRESSING},
    {"clr", 5, 10, 0, DIRECT_INDEX_REGISTER_ADDRESSING},
    {"not", 5, 11, 0, DIRECT_INDEX_REGISTER_ADDRESSING},
    {"inc", 5, 12, 0, DIRECT_INDEX_REGISTER_ADDRESSING},
    {"dec", 5, 13, 0, DIRECT_INDEX_REGISTER_ADDRESSING},
    {"jmp", 9, 10, 0, DIRECT_ADDRESING | INDEX_ADDRESSING},
    {"bne", 9, 11, 0, DIRECT_ADDRESING | INDEX_ADDRESSING},
    {"jsr", 9, 12, 0, DIRECT_ADDRESING | INDEX_ADDRESSING},
    {"red", 12, 0, 0, DIRECT_INDEX_REGISTER_ADDRESSING},
    {"prn", 13, 0, 0, ALL_ADDRESSING_METHODS},
    {"rts", 14, 0, 0, 0},
    {"stop", 15, 0, 0, 0}
    };
    
    return commands;
}


const AddressMethodWords *wordsPerAddressMethods()
{
    AddressMethodWords addressMethodsWordsDict[ADDRESS_METHOD_AMOUNT] = {
    {IMMEDIATE_ADDRESING, 1, IMMEDIATE},
    {DIRECT_ADDRESING, 2, DIRECT},
    {INDEX_ADDRESSING, 2, INDEX},
    {REGISTER_DIRECT_ADDRESSING, 0, REGISTER_DIRECT}
    };

    return addressMethodsWordsDict;
}