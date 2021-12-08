#ifndef POSTFIX
#define POSTFIX
#include "parser.h"
#include<stdio.h>
#include<ctype.h>

Token* pop(Token** stack, int* top);
int priority(Token* x);
int convertToPost(Parser* par, ParseTree* current, TokenType EO_Expr);
#endif