#include <stdio.h>
#include "Lexer.h"
#include "Debug.h"
#include "SymbolTable.h"
#include "Lexer.h"
#include "Parser.h"
#include "ParseTree.h"
#include "interface.h"
#include "Visitor.h"


void testVisitor(Parser* par) {
    Visitor vis;
    Error_Codes code = newVisitor(&vis, par);
    if (code != VIS_OK) {
        // Print some kind of error
        return;
    }

    visitAll(&vis);
}

void testParser(char* code) {
    Lexer lex;
    newLexer(&lex, code);
    Parser par;
    newParser(&par, &lex);
    //startParsing(&par);
    testVisitor(&par);
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

//void testSymbolTable() {
//    Table tab;
//    newTable(&tab);
//    TABLE_VALUE val, val2;
//    struct arg argnew = makeArg("x", "int");
//    struct variable var = makeVariable("int", "5");
//    struct function func = makeFunction(&argnew, 1, "float");

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


int main( int argc, char** argv ) {

    testInterface(argc, argv);
    return 0;
}
