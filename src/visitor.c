#include "../include/visitor.h"

ast* make_binaryExp ( char* operator_, ast* left, ast* right ) {
  ast* e = (ast*) malloc(sizeof(ast));
  e->tag = binary_exp;
  e->binary.operator_ = operator_;
  e->binary.left = left;
  e->binary.right = right;
  return e;
}

ast* make_variableExp(char* name, TYPES type, ast* value) {
    ast* e = (ast*)malloc(sizeof(ast));
    e->tag = variable_exp;
    e->variable_creation.name = (char*)malloc(sizeof(char)*(strlen(name)+1));
    strcpy(e->variable_creation.name, name);
    e->variable_creation.type = type;
    e->variable_creation.exp = value;
    return e;
}
ast* make_integerExp(int value) {
    ast* e = (ast*)malloc(sizeof(ast));
    e->tag = integer_exp;
    e->integer.value = value;
    return e;
}