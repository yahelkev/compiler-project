#include <stdio.h>
#include "../inc/Lexer.h"
#include "../inc/Debug.h"

int main( void ) {

    Lexer lex;
    char* text = "13 + 2";
    newLexer(&lex, text);
    Token token;
    while ((token = scanLexer(&lex)).type != TOKEN_EOF)
    {
        printToken(&token);
    } 
    return 0;
}