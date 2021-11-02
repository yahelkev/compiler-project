#include <stdio.h>
#include "../inc/Lexer.h"
#include "../inc/Debug.h"

int main( void ) {

    Lexer lex;
    newLexer(&lex, "1 + 2");
    Token token;
    while ((token = scanLexer(&lex)).type != TOKEN_EOF)
    {
        printToken(&token);
    } 
    return 0;
}