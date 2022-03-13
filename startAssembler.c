#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
// #include "preprocessor.h"
#include "utils.h"
#include "assembler.h"
#include "constants.h"


int main(int argc, char *argv[])
{
    // int *preprocessedFile;
    // ActionNode *mov;
    // ActionNode **instructions;
    // instructions = createAllInstructions();
    int i;
    // for (i=0; i < 16;i++)
    //     printf("node: %s: %u -> %u -> %u -> %u\n", instructions[i]->name, instructions[i]->funct, instructions[i]->opcode, instructions[i]->sourceAdressingMethods, instructions[i]->targetAdressingMethods);
    // printf("symbolTable node is %s -> %d -> %d -> %s\n", table->symbolName, table->value, table->attribute1, table->next->symbolName);
    
    SymbolNode *symbolTable = (SymbolNode *) malloc(sizeof(SymbolNode)); // createNewSymbol("first", 200, ENTRY);
    int result, res2;
    // result = insertSymbolToTable(symbolTable, instructions, "srut", 100, CODE);
    // if (symbolTable->next == NULL) printf("next name - %s", symbolTable->symbolName);
    //res2 = insertSymbolToTable(symbolTable, instructions, "tik", 105, EMPY);
    //printf("result is %d, head of table is %s- next value is %s", result, ptr->symbolName, ptr->next->symbolName);
    // while (--argc > 0)
    // {
    //     //preprocessedFile = processFile(*++argv);
    //     printf("%s\n", *++argv);
    // }

    return 1;   
}