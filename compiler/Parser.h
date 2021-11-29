#ifndef PARSER_H
#define PARSER_H

#include "Lexer.h"

typedef struct Parser {
	Lexer* lex;
	Token* current;
	Token* pre;

	unsigned char error;
	unsigned char panic;
}Parser;

//forward declare the precedent rules, from low to high
typedef enum {
	PREC_NONE,
	PREC_ASSIGNMENT,
	PREC_TERNARY,
	PREC_OR,
	PREC_AND,
	PREC_EQUALITY,
	PREC_COMPARISON,
	PREC_TERM,
	PREC_FACTOR,
	PREC_UNARY,
	PREC_CALL,
	PREC_PRIMARY,
} Precedence;


void newParser(Parser* par, Lexer* lex);
void scanParser(Parser* par);
void parserAdvance(Parser* par);
void statement(Parser* par);
void parseVariableCreation(Parser* par);
void error(Parser* parser, Token* token, const char* message);
void synchronize(Parser* parser);
#endif // !PARSER_H