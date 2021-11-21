#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>
#include <stdlib.h>
#include "KeywordTypes.h"
#include "Lexer.h"
#include "SymbolTable.h"

void printToken(Token* token);
void printTableValue(TABLE_VALUE* val);
#endif // !DEBUG_H