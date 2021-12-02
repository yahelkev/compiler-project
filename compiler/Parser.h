#ifndef PARSER_H
#define PARSER_H

#include "Lexer.h"
#include "ParseTree.h"
typedef struct Parser {
	Lexer* lex;
	Token* current;
	Token* pre;

	int error;
	int panic;

	ParseTree* mainTree;
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
void startParsing(Parser* par);
void scanParser(Parser* par, ParseTree* current);
void parserAdvance(Parser* par);
int statement(Parser* par, ParseTree* current);
void expression(Parser* par, ParseTree* current);
void error(Parser* parser, Token* token, const char* message);
void synchronize(Parser* parser);

// Templates
int parseVariableCreation(Parser* par, ParseTree* current);
#endif // !PARSER_H