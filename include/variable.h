#ifndef VARIABLE_H
#define VARIABLE_H

enum types {
    NUMBER = 0,
    STR,
    BOOL
};



typedef struct VARIABLE_S {
    char* varName;
    int type;
    char* value;
}VARIABLE;

#endif // !VARIABLE_H