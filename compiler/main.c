#include <stdio.h>
#include "Lexer.h"
#include "Debug.h"
#include "SymbolTable.h"
#include "Lexer.h"
#include "Parser.h"
#include "ParseTree.h"
#include "interface.h"

void testParser(char* code) {
    Lexer lex;
    newLexer(&lex, code);
    Parser par;
    newParser(&par, &lex);
    startParsing(&par);
    printParseTree(par.mainTree);
    par.mainTree->freeParseTree(par.mainTree);
}

void printSpacer(int range, char spacer) {
    for (size_t i = 0; i < range; i++) putchar(spacer);
}
void testLexer(Lexer* lex, char* rawCode) {
    printf("\nCode\tStart\tEnd\tAdditional:\n");
    printSpacer(45, '=');
    putchar('\n\n');
    newLexer(lex, rawCode);
    Token* token;
    while ((token = scanLexer(lex))->type != TOKEN_EOF) {
        printToken(token);
        free(token);
    }
    printSpacer(45, '-');
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
    newValue(&val, VARIABLE_TAG, &var, 1, 5);
    newValue(&val2, FUNCTION_TAG, &func, 1, 10);
    printTableValue(&val);
    printTableValue(&val2);
}

void testInterface(int argc, char** argv) {
    char* srcFileName = NULL;
    int flag = handleInput(argc, argv, &srcFileName);
    char* fileContent = getFileContent(srcFileName);
    testParser(fileContent);
}



void testParseTree() {
    ParseTree* tree = newTree(FUNCTION_PARSE, NULL);
    Token* toke = (Token*)malloc(sizeof(Token));
    toke->column = 3;
    toke->line = 1;
    toke->length = 3;
    toke->type = TOKEN_IDENTIFIER;
    toke->lexeme = (char*)malloc(sizeof(char) * 4);
    strcpy(toke->lexeme, "add");
    ParseTree* iden = newTree(IDENTIFIER_PARSE, toke);
    tree->addChild(tree, iden);
    printParseTree(tree);
    printf("\n==========\n");
    printParseTree(iden);
    tree->freeParseTree(tree);

}


int main( int argc, char** argv ) {

    Lexer lex;
    newLexer(&lex, "int x = (2) > (3 -  (9 - 8)*1)\n");
    Parser par;
    newParser(&par, &lex);
    startParsing(&par);
    printParseTree(par.mainTree);
    
    return 0;
}