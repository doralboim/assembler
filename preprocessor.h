char *processFile(char *fileName);
struct macro
{
    int name;
    char *data;
    struct macro *next;
};
typedef struct macro *Macroptr;
typedef struct macro {
    int name;
    char *data;
    Macroptr next;
} MacroNode;
