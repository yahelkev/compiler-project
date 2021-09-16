#ifndef VARIABLE_H
#define VARIABLE_H
#include <stdlib.h>
#include <string.h>
enum types {
    NUMBER = 0,
    STR,
    BOOL
};



typedef struct VARIABLE_ {
    char* varName;
    int type;
    char* value;

    void (*free)(struct VARIABLE_*);
    char* (*str)(struct VARIABLE_*);
}VARIABLE;

VARIABLE* variable(char* name, char* value, int type);
void __FREEVARIABLE__(VARIABLE* var);
char* __STRVARIABLE__(VARIABLE* var);

#endif // !VARIABLE_H