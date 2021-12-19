#ifndef POSTFIX_H
#define POSTFIX_H

#include "Parser.h"
#include <stdio.h>
#include <ctype.h>

#define CON_PRIORITY 1
#define PLUS_MINUS_PRIORITY 2
#define MUL_DIV_PRIORITY 3

#define MAX_STACK_SIZE 100
#define EMPTY_STACK -1

Token* pop(Token** stack, int* top);
int priority(Token* x);
int convertToPost(Parser* par, ParseTree* current, TokenType EO_Expr);
#endif // !POSTFIX_H