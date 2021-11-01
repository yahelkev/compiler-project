#ifndef LEXER_H
#define LEXER_H
#include <string.h>
#include <stdlib.h>

typedef struct Lexer {
    
    int line, column, index, size;
    char current;
    char* text;

}Lexer;

// Iniatiates a lexer with blank values
void newLexer(Lexer* lex, char* text);
// Peeks next character
char peek(Lexer* lex);
// Shows current character
char show(Lexer* lex);
/**
 * @lex -
 Lexer to advance
 * @stringFlag -
 Flag that represents if we are in the middle in a string,
  and wheter to move to a new line if '\n' is detected
**/
void advance(Lexer* lex, int stringFlag);





#endif // !LEXER_H