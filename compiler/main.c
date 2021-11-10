#include <stdio.h>
#include "interface.h"
#include "Lexer.h"
#include "Debug.h"

void test(Lexer* lex, char* rawCode);


void  main(int argc, char* argv[])
{
    char* srcFileName = NULL;
    int flag = handleInput(argc, argv, &srcFileName);
    printf("flag: %d\n", flag);
    printf("file: %s\n", srcFileName);
    char* fileContent = getFileContent(srcFileName);

    Lexer lex;
    test(&lex, fileContent);

    if (fileContent) {
        free(fileContent);
    }

}

void test(Lexer* lex, char* rawCode) {
    newLexer(lex, rawCode);
    Token token;
    while ((token = scanLexer(lex)).type != TOKEN_EOF) {
        printToken(&token);
    }
    for (size_t i = 0; i < 15; i++) putchar('-');
    putchar('\n');
    return;
}
