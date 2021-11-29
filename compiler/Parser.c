#include "Parser.h"

void newParser(Parser* par, Lexer* lex) {
	par->lex = lex;
	par->pre = par->current = NULL;
}

void statement(Parser* par) {
	// Grammer rules and possiblities
	switch (par->current->type) {
	case TOKEN_INT_V:
	case TOKEN_FLOAT_V:
	case TOKEN_CHAR_V:
	case TOKEN_STRING_V:
		advance(par);
		parseVariableCreation(par);
	}
}

void synchronize(Parser* parser) {
	while (parser->current->type != TOKEN_EOF) {
		switch (parser->current->type) {
			//these tokens can start a line
		case TOKEN_FOR:
		case TOKEN_IF:
		case TOKEN_PRINT:
		case TOKEN_RETURN:
		case TOKEN_INT_V:
		case TOKEN_FLOAT_V:
		case TOKEN_CHAR_V:
		case TOKEN_STRING_V:
		case TOKEN_WHILE:
			parser->panic = false;
			return;

		default:
			advance(parser);
		}
	}
}

void scanParser(Parser* par) {
	if (!par->current && !par->pre) {
		parserAdvance(par);
	}
	statement(par);
}

//parsing utilities
void error(Parser* parser, Token* token, const char* message) {
	//keep going while panicing
	if (parser->panic) return;

	fprintf(stderr, "[Line %d] Error", token->line);

	//check type
	if (token->type == TOKEN_EOF) {
		fprintf(stderr, " at end");
	}

	else {
		fprintf(stderr, " at '%.*s'", token->length, token->lexeme);
	}

	//finally
	fprintf(stderr, ": %s\n", message);
	parser->error = 1;
	parser->panic = 1;
}

void parserAdvance(Parser* par) {
	Token temp = scanLexer(par->lex);
	par->pre = par->current;
	par->current = &temp;

	if (par->current->type == TOKEN_ERROR) {
		error(par, par->current, "Lexer error");
	}

}

void parseVariableCreation(Parser* par) {

}
