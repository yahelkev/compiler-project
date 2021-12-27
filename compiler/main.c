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

void testInterface(int argc, char** argv) {
    char* srcFileName = NULL;
    int flag = handleInput(argc, argv, &srcFileName);
    char* fileContent = getFileContent(srcFileName);
    //printf("%s\n", fileContent);
    testParser(fileContent);
}




int main( int argc, char** argv ) {

    testInterface(argc, argv);
    return 0;
}
