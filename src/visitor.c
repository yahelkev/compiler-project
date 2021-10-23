#include "../include/visitor.h"

ast* make_binaryExp ( char* operator_, ast* left, ast* right ) {
  ast* e = (ast*) malloc(sizeof(ast));
  e->tag = binary_exp;
  e->binary.operator_ = operator_;
  e->binary.left = left;
  e->binary.right = right;
  return e;
}

ast* make_creationvariableExp(char* name, TYPES type, ast* value) {
    ast* e = (ast*)malloc(sizeof(ast));
    e->tag = variable_exp;
    e->variable_creation.name = (char*)malloc(sizeof(char)*(strlen(name)+1));
    strcpy(e->variable_creation.name, name);
    e->variable_creation.type = type;
    e->variable_creation.exp = value;
    return e;
}

ast* make_variableExp(char* name, ast* value) {
    // type = gettypefromsymboltable(name)
    // e->variable_creation.type = type;
    TYPES type = 0;
    return make_creationvariableExp(name, type, value);
}
ast* make_integerExp(int value) {
    ast* e = (ast*)malloc(sizeof(ast));
    e->tag = integer_exp;
    e->integer.value = value;
    return e;
}




CODE parse_variableExp(ast* tree) {
    // Check for symbol tabel if variable name exists in scope
    // tree->variable_creation.name in symbol_table
    switch(tree->variable_creation.exp->tag) {
        case binary_exp: return parse_binaryExp(tree->variable_creation.exp);
        case integer_exp: return parse_integerExp(tree->variable_creation.exp);
        default: printf("error unknown tag\n"); return 0;
    }
    
}


CODE parse_binaryExp(ast* tree) {
    int load = 0;
    switch(tree->binary.left->tag) {
        case integer_exp: load = parse_integerExp(tree->binary.left); break;
        case binary_exp: load = parse_binaryExp(tree->binary.left); break;
        default: printf("error unknown tag\n"); return 0;
    }
    switch(tree->binary.right->tag) {
        case integer_exp: load = load & parse_integerExp(tree->binary.right);
        case binary_exp: load = load & parse_binaryExp(tree->binary.right);
        default: printf("error unknown tag\n"); return 0;
    }
    if(!load) return load;
    if(tree->binary.right->tag == integer_exp &&
        tree->binary.right->integer.value == 0 &&
        tree->binary.operator_ = "/")
            return load ^ load;
    return load;
}
CODE parse_integerExp(ast* tree) {
    return 1;
}