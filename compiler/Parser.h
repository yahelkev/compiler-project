#ifndef PARSER_H
#define PARSER_H

#include "Lexer.h"

typedef struct Parser {
	Lexer* lex;
	Token* current;
	Token* pre;
}Parser;

void newParser(Parser* par, Lexer* lex);
void scanParser(Parser* par);
void parserAdvance(Parser* par);
void statement(Parser* par);
#endif // !PARSER_H