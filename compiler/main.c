#include <stdio.h>
#include "Lexer.h"
#include "Debug.h"
#include "SymbolTable.h"
#include "Lexer.h"
#include "Parser.h"
#include "ParseTree.h"
#include "interface.h"
#include "Visitor.h"
#include "CodeGen.h"



int main( int argc, char** argv ) {

    char* srcFileName = NULL;
    handleInput(argc, argv, &srcFileName);
    
    Lexer lex;
    
    Parser par;
    Visitor vis;
    CodeGen gen;

    newLexer(&lex, getFileContent(srcFileName));
    newParser(&par, &lex);
    Error_Codes code = newVisitor(&vis, &par);
    if (code != VIS_OK) {
        c(RED);
        fprintf(stderr, "Error while parsing trees\n");
        c(GRAY);
        return 0;
    }
    visitAll(&vis);
    if (vis.error) {
        c(RED);
        fprintf(stderr, "Error while parsing trees\n");
        c(GRAY);
        return 0;
    }
    newCodeGen(&gen, srcFileName, vis.par->mainTree, vis.par->table);
    gen.filePointer = CreateBlankFile(gen.filePath);
    Generate(&gen);
    emitAsm(&gen);
    return 0;
}
