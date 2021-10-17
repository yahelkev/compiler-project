#ifndef PARSER_H
#define PARSER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "dict.h"

// Might change return type to custom type refering if the parsing was succseful or not
typedef struct PARSER {
    dict signatureList; // symCodes and function pointers 
}Parser;
void begin(Token* tk);


#endif // !PARSER_H