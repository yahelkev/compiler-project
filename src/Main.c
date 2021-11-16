#include <stdio.h>
#include "../inc/Lexer.h"
#include "../inc/Debug.h"
#include "../inc/SymbolTable.h"

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

    // Lexer lex;
    // test(&lex, "fn add(int a, int b) -> int {\n\treturn a + b\n}");
    // test(&lex, "int x = z + 1");
    // test(&lex, "float y = x + 13 * 11");

    Table tab;
    newTable(&tab);
    TABLE_VALUE val;
    struct variable var = makeVariable("int");
    newValue(&val, VARIABLE_TAG, &var , 1, 5);
    printTableValue(&val);

    
    return 0;
}