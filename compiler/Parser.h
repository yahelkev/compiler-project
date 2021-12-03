#ifndef PARSER_H
#define PARSER_H

#include "Lexer.h"
#include "ParseTree.h"
typedef struct Parser {
	Lexer* lex;
	Token* current;
	Token* pre;

	int error; // Tells if there was an error in the code, so to figure out if to continue to the visitor stage or not
	int panic; // Tells if we are in a middle of an error and wheter to ignore new errors or not

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

// Creates a new parser
void newParser(Parser* par, Lexer* lex);
// Starts the parsing proccess on a select parser object
void startParsing(Parser* par);
// Lists option of different possible parse trees
void scanParser(Parser* par, ParseTree* current);
// Advances the parser to theh next token coming from the lexer
void parserAdvance(Parser* par);
// Liists possible creation statements of parse tress
void statement(Parser* par, ParseTree* current);
// Prints error to screen ( user )
void error(Parser* parser, Token* token, const char* message);
// Comes usually after error function, skips to next possible starting token to start a parse tree
void synchronize(Parser* parser);

// Templates
// Function to create a variable creation parse tree
void parseVariableCreation(Parser* par, ParseTree* current);
#endif // !PARSER_H