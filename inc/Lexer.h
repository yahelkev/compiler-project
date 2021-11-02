#ifndef LEXER_H
#define LEXER_H
#include <string.h>
#include <stdlib.h>
#include "token_type.h"


typedef int bool
#define false NULL
#define true !false

typedef struct Token {
	TokenType type;
	char* lexeme;
	int length;
	int line, column;
} Token;

typedef struct Lexer {
    
    int line, column, index, size;
    char current;
    char* text;

}Lexer;

// Iniatiates a lexer with blank values
static void newLexer(Lexer* lex, char* text);
bool isAtEnd(Lexer* lex);
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
/*
    Main start function
    * Will change return type to a custom token list type
    * Will probably add a flagging option of some sort
*/
void start(Lexer* lex);

bool isDigit(char c);
bool isAlpha(char c);
bool isIdentifier(char c);
Token MakeToken(TokenType type, char* lexeme, int line, int column);
Token scanLexer(Lexer* lex);
void eatWhiteSpace(Lexer* lex);
Token makeNumber(Lexer* lexer);
Token makeString(Lexer* lexer, char terminator);
bool match(Lexer* lexer, char c);
#endif // !LEXER_H