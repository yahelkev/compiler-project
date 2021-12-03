#include <stdio.h>
#include "Lexer.h"
#include "Debug.h"
#include "SymbolTable.h"
#include "ParseTree.h"
#include "Parser.h"

void testLexer( Lexer* lex, char* rawCode ) {
    newLexer(lex, rawCode);
    Token* token;
    while ((token = scanLexer(lex))->type != TOKEN_EOF) {
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

void testParseTree() {
    ParseTree* tree = newTree(FUNCTION_PARSE, NULL);
    Token* toke = (Token*)malloc(sizeof(Token));
    toke->column = 3;
    toke->line = 1;
    toke->length = 3;
    toke->type = TOKEN_IDENTIFIER;
    toke->lexeme = (char*)malloc(sizeof(char) * 4);
    strncpy(toke->lexeme, "add", 3);
    ParseTree* iden = newTree(IDENTIFIER_PARSE, toke);
    tree->addChild(tree, iden);
    printParseTree(tree);
    printf("\n==========\n");
    printParseTree(iden);
    tree->freeParseTree(tree);

}

int main( void ) {

     //Lexer lex;
     //testLexer(&lex, "fn add(int a, int b) -> int {\n\treturn a + b\n}");
     //testLexer(&lex, "int x = z + 1");
    // test(&lex, "float y = x + 13 * 11");

    // testSymbolTable();
    //testParseTree();
    Lexer lex;
    newLexer(&lex, "int x = 1");
    Parser par;
    newParser(&par, &lex);
    startParsing(&par);
    //printParseTree(par.mainTree);
    printTree(par.mainTree);



    
    return 0;
}