#ifndef LEXER_H
#define LEXER_H
#include <string.h>
#include <stdlib.h>
#include "TokenType.h"
#include "KeywordTypes.h"

#define bool int
#define false 0
#define true 1


#define DONT_OVERWRITE_BASELINE -1
typedef struct Token {
	TokenType type;
	char* lexeme;
	int length;
	int line, column;
} Token;

typedef struct Lexer {
    
    int line, column, index, size, start;
    char current;
    char* text;

}Lexer;

void cleanLexer(Lexer* lex);
// Iniatiates a lexer with blank values
void newLexer(Lexer* lex, char* text);
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
char advance(Lexer* lex);

bool isDigit(char c);
bool isAlpha(char c);
bool isIdentifier(char c);
Token* makeToken(Lexer* lex, TokenType type);
Token* scanLexer(Lexer* lex);
void eatWhiteSpace(Lexer* lex);
Token* makeNumber(Lexer* lexer);
Token* makeString(Lexer* lexer, char terminator);
Token* makeKeywordOrIdentifier(Lexer* lexer);
Token* makeErrorToken(Lexer* lex, char* msg, int startLine);
bool match(Lexer* lexer, char c);
void freeToken(Token* toke);
void freeLexer(Lexer* lex);
#endif // !LEXER_H