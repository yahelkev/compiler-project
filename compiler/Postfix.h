#ifndef POSTFIX
#define POSTFIX
#include "parser.h"
#include<stdio.h>
#include<ctype.h>

#define CON_PRIORITY 1
#define PLUS_MINUS_PRIORITY 2
#define MUL_DIV_PRIORITY 3

Token* pop(Token** stack, int* top);
int priority(Token* x);
int convertToPost(Parser* par, ParseTree* current, TokenType EO_Expr);
#endif