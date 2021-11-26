#include "Parser.h"

void newParser(Parser* par, Lexer* lex) {
	par->lex = lex;
	par->pre = par->current = NULL;
}

void statement(Parser* par) {
	// Grammer rules and possiblities
	
}

void scanParser(Parser* par) {
	if (!par->current && !par->pre) {
		parserAdvance
		(par);
	}
	statement(par);
}

void parserAdvance(Parser* par) {
	Token temp = scanLexer(par->lex);
	par->pre = par->current;
	par->current = &temp;
}
