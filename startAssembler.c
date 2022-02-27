#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "preprocessor.h"
#include "instructions.h"
#include "utils.h"
#include "assembler.h"
#include "constants.h"


int main(int argc, char *argv[])
{
    // int *preprocessedFile;
    // InstructionNode *mov;
    InstructionNode **instructions;
    instructions = createAllInstructions();

    int i;
    // for (i=0; i < 16;i++)
    //     printf("node: %s: %u -> %u -> %u -> %u\n", instructions[i]->name, instructions[i]->funct, instructions[i]->opcode, instructions[i]->sourceAdressingMethods, instructions[i]->targetAdressingMethods);
    // printf("symbolTable node is %s -> %d -> %d -> %s\n", table->symbolName, table->value, table->attribute1, table->next->symbolName);
    
    SymbolNode *symbolTable = createNewSymbol("first", 200, ENTRY);
    int result;
    result = insertSymbolToTable(&symbolTable, instructions, "srut", 100, 1);
    // printf("symbolTable node is %s -> %d -> %d -> %s\n", symbolTable->symbolName, symbolTable->value, symbolTable->attribute1, symbolTable->next->symbolName);
    int count = 0;
    while (symbolTable->value != 0)
    {
        printf("node number %d, symbol is %s value is %d\n", count, symbolTable->symbolName, symbolTable->value);
        count++;
        symbolTable = symbolTable->next;
    }
    printf("result is %d, head of table is %s- next value is %s", result, symbolTable->symbolName, symbolTable->next->symbolName);
    // while (--argc > 0)
    // {
    //     //preprocessedFile = processFile(*++argv);
    //     printf("%s\n", *++argv);
    // }

    return 1;   
}