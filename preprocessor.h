#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

char *processFile(char *fileName);
typedef struct macro_node
{
    char name[MAX_MACRO_NAME];
    char **data;
    int dataLength;
    int rowsCount;
    struct macro_node *next;
} MacroNode;

#endif