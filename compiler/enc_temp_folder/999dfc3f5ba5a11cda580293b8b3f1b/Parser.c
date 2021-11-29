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

int statement(Parser* par, ParseTree* current) {
	// Grammer rules and possiblities
	switch (par->current->type) {
	case TOKEN_INT_V:
	case TOKEN_FLOAT_V:
	case TOKEN_CHAR_V:
	case TOKEN_STRING_V:
		parserAdvance(par);
		return parseVariableCreation(par, current);
	}
	return 0;
}

int expression(Parser* par, ParseTree* current) {
	// In the future will create a branch to explore this path
	// As of right now this will be very basic without arithmetic
	// Also will add function calls here when we get to that template
	switch (par->current->type) {
		case TOKEN_IDENTIFIER: {
			ParseTree* iden = newTree(IDENTIFIER_PARSE, par->current);
			current->addChild(current, iden);
			return 1;
		}
		case TOKEN_NUMBER:
		case TOKEN_STRING: {
			ParseTree* iden = newTree(ATOMIC_PARSE, par->current);
			current->addChild(current, iden);
			return 1;
		}
	}
	return 0;
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
	if(!statement(par, current)) 
		expression(par, current);
	// int x = 5
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
	//freeToken(par->pre);
	par->pre = par->current;
	par->current = scanLexer(par->lex);

	if (par->current->type == TOKEN_ERROR) {
		error(par, par->current, "Lexer error");
	}

}

int parseVariableCreation(Parser* par, ParseTree* current) {
	ParseTree* mainTree = newTree(VARIABLE_PARSE, NULL);
	mainTree->addChild(mainTree, newTree(PARSE_INT_V, par->pre));
	mainTree->addChild(mainTree, newTree(IDENTIFIER_PARSE, par->current));
	parserAdvance(par);
	switch (par->current->type) {
	case TOKEN_PLUS_EQUAL:
		mainTree->addChild(mainTree, newTree(PARSE_PLUS_EQUAL, par->current));
		break;
	case TOKEN_EQUAL:
		mainTree->addChild(mainTree, newTree(PARSE_EQUAL, par->current));
		break;
	case TOKEN_MINUS_EQUAL:
		mainTree->addChild(mainTree, newTree(PARSE_MINUS_EQUAL, par->current));
		break;
	case TOKEN_STAR_EQUAL:
		mainTree->addChild(mainTree, newTree(PARSE_STAR_EQUAL, par->current));
		break;
	case TOKEN_SLASH_EQUAL:
		mainTree->addChild(mainTree, newTree(PARSE_SLASH_EQUAL, par->current));
		break;
	case TOKEN_MODULO_EQUAL:
		mainTree->addChild(mainTree, newTree(PARSE_MODULO_EQUAL, par->current));
		break;
	default:
		error(par, par->current, "Invalid Syntax");

	}
	parserAdvance(par);
	ParseTree* treeExpression = newTree(EXPRESSION_PARSE, NULL);
	scanParser(par, treeExpression);
	mainTree->addChild(mainTree, treeExpression);
	
	current->addChild(current, mainTree);
	parserAdvance(par);
	return 1;
}
