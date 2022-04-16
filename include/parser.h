#ifndef PARSER_H
#define PARSER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "dict.h"

// Might change return type to custom type refering if the parsing was succseful or not
typedef struct CALL {
    int check;
    Token* tk;
}Call;


typedef struct PARSER {
    struct _dict* signatureList; // symCodes and function pointers
    void (*destroy)(struct PARSER* par);
}Parser;

Parser* new_parser();
void __FREE_PARSER(Parser* par);
void begin(Token* tk, Parser* par);

Call* new_call(Token* tk, int check);


#endif // !PARSER_H