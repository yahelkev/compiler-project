#include <stdio.h>
#include "../inc/Lexer.h"
#include "../inc/Debug.h"

void test( Lexer* lex, char* rawCode ) {
    newLexer(lex, rawCode);
    Token token;
    while ((token = scanLexer(lex)).type != TOKEN_EOF) {
        printToken(&token);
    }
    for (size_t i = 0; i < 15; i++) putchar('-');
    putchar('\n');
    return;
}   


int main( void ) {

    Lexer lex;
    test(&lex, "fn add(int a, int b) -> int {\n\treturn a + b\n}");
    test(&lex, "int x = z + 1");
    test(&lex, "float y = x + 13 * 11");
    return 0;
}