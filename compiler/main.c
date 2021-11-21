#include <stdio.h>
#include "Lexer.h"
#include "Debug.h"
#include "SymbolTable.h"

void testLexer( Lexer* lex, char* rawCode ) {
    newLexer(lex, rawCode);
    Token token;
    while ((token = scanLexer(lex)).type != TOKEN_EOF) {
        printToken(&token);
    }
    for (size_t i = 0; i < 15; i++) putchar('-');
    putchar('\n');
    return;
}   

void testSymbolTable() {
    Table tab;
    newTable(&tab);
    TABLE_VALUE val, val2;
    struct arg argnew = makeArg("x", "int");
    struct variable var = makeVariable("int", "5");
    struct function func = makeFunction(&argnew, 1, "float");
    newValue(&val, VARIABLE_TAG, &var , 1, 5);
    newValue(&val2, FUNCTION_TAG, &func, 1, 10);
    printTableValue(&val);
    printTableValue(&val2);
}

int main( void ) {

    // Lexer lex;
    // test(&lex, "fn add(int a, int b) -> int {\n\treturn a + b\n}");
    // test(&lex, "int x = z + 1");
    // test(&lex, "float y = x + 13 * 11");

    // testSymbolTable();
    

    
    return 0;
}