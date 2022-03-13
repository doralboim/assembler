#ifndef PREPROCESSOR_H

char *processFile(char *fileName);
struct Macro
{
    int name;
    char *data;
    struct Macro *next;
};
typedef struct Macro *Macroptr;
typedef struct Macro {
    int name;
    char *data;
    Macroptr next;
} MacroNode;

#define PREPROCESSOR_H
#endif