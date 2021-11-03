#include <stdio.h>
#include "../inc/Lexer.h"
#include "../inc/Debug.h"

int main( void ) {

    Lexer lex;
    char* text = "int x = 3";
    newLexer(&lex, text);
    Token token;
    while ((token = scanLexer(&lex)).type != TOKEN_EOF)
    {
        printToken(&token);
    } 
    return 0;
}