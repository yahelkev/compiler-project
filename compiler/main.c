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
    //printParseTree(par.mainTree); 
    par.mainTree->freeParseTree(par.mainTree);
}


void testInterface(int argc, char** argv) {
    char* srcFileName = NULL;
    int flag = handleInput(argc, argv, &srcFileName);
    char* fileContent = getFileContent(srcFileName);
    testParser(fileContent);
}


int main( int argc, char** argv ) {

    testInterface(argc, argv);
    return 0;
}
