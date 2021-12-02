#include "Parser.h"

void newParser(Parser* par, Lexer* lex) {
	par->lex = lex;
	par->pre = par->current = NULL;
	par->mainTree = newTree(MAIN_PARSE, NULL);
	parserAdvance(par);
}

void startParsing(Parser* par) {
	while(par->current->type != TOKEN_EOF) {
		scanParser(par, par->mainTree);
	}
}

void statement(Parser* par, ParseTree* current) {
	// Grammer rules and possiblities
	switch (par->current->type) {
	case TOKEN_INT_V:
	case TOKEN_FLOAT_V:
	case TOKEN_CHAR_V:
	case TOKEN_STRING_V:
		parserAdvance(par);
		parseVariableCreation(par, current);
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
			parserAdvance(parser);
		}
	}
}

void scanParser(Parser* par, ParseTree* current) {
	statement(par, current);
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
	par->pre = par->current;
	par->current = scanLexer(par->lex);

	if (par->current->type == TOKEN_ERROR) {
		error(par, par->current, "Lexer error");
	}

}

void parseVariableCreation(Parser* par, ParseTree* current) {
	ParseTree* mainTree = newTree(VARIABLE_PARSE, NULL);
	ParseTree* tree = newTree(INT_PARSE, par->pre);
	ParseTree* tree1 = newTree(IDENTIFIER_PARSE, par->current);
	parserAdvance(par);
	ParseTree* tree2 = newTree(ASSIGN_PARSE, par->current);
	parserAdvance(par);
	ParseTree* tree3 = newTree(EXPRESSION_PARSE, NULL);
	ParseTree* tree4 = newTree(ATOMIC_PARSE, par->current);
	tree3->addChild(tree3, tree4);
	mainTree->addChild(mainTree, tree);
	mainTree->addChild(mainTree, tree1);
	mainTree->addChild(mainTree, tree2);
	mainTree->addChild(mainTree, tree3);
	
	current->addChild(current, mainTree);
	parserAdvance(par);
	return;
}
