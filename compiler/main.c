#include <stdio.h>
#include "Lexer.h"
#include "Debug.h"
#include "SymbolTable.h"
#include "interface.h"
void printSpacer(int range, char spacer) {
    for (size_t i = 0; i < range; i++) putchar(spacer);
}
void testLexer( Lexer* lex, char* rawCode ) {
    printf("\nCode\tStart\tEnd\tAdditional:\n");
    printSpacer(45, '=');
    putchar('\n\n');
    newLexer(lex, rawCode);
    Token token;
    while ((token = scanLexer(lex)).type != TOKEN_EOF) {
        printToken(&token);
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
    newValue(&val, VARIABLE_TAG, &var , 1, 5);
    newValue(&val2, FUNCTION_TAG, &func, 1, 10);
    printTableValue(&val);
    printTableValue(&val2);
}

void testInterface(int argc, char* argv[]) {
    char* srcFileName = NULL;
    int flag = handleInput(argc, argv, &srcFileName);
    printf("flag: %d\n", flag);
    printf("file: %s\n", srcFileName);
    char* fileContent = getFileContent(srcFileName);
    Lexer lex;
    testLexer(&lex, fileContent);
    if (srcFileName) {
        free(srcFileName);
    }

}

int main(int argc, char* argv[]) {

     Lexer lex;
     testLexer(&lex, "fn add(int a, int b) -> int {\n\treturn a + b\n}");
     testLexer(&lex, "int x = z + 1");
     testLexer(&lex, "float y = x + 13 * 11");

     testSymbolTable();
    
     testInterface(argc, argv);
    
    return 0;
}
