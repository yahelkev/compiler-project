#ifndef VISITOR_H
#define VISITOR_H
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "types.h"
typedef int CODE;
typedef struct Exp {
    enum {
        variable_exp, binary_exp, integer_exp
    } tag;

    union {
        struct {
            char* name;
            TYPES type;
            struct Exp* exp;
        } variable_creation; // makeVariableExp | make_creationvariableExp

        struct {
            char * operator_;
            struct Exp* left;
            struct Exp * right;
        }binary; // makeBinaryExp

        struct {
            int value;
        }integer; // makeIntegerExp
    };
}ast;


ast* make_binaryExp ( char* operator_, ast* left, ast* right );
ast* make_creationvariableExp(char* name, TYPES type, ast* value);
ast* make_variableExp(char* name, ast* value);
ast* make_integerExp(int value);


CODE parse(ast* tree);

CODE parse_variableExp(ast* tree);
CODE parse_binaryExp(ast* tree);
CODE parse_integerExp(ast* tree);
#endif // !VISITOR_H