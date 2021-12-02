#include "Parser.h"

void newParser(Parser* par, Lexer* lex) {
	par->lex = lex;
	par->pre = par->current = NULL;
	par->error = par->panic = 0;
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
	case TOKEN_STRING_V:
		parserAdvance(par);
		return parseVariableCreation(par, current);
	default:
		error(par, par->current, "Invalid Syntax");
		synchronize(par);
	}
	return 0;
}

int expression(Parser* par, ParseTree* current) {
	// In the future will create a branch to explore this path
	// As of right now this will be very basic without arithmetic
	// Also will add function calls here when we get to that template
	while (par->current->type != TOKEN_END_LINE && par->current->type != TOKEN_EOF)
	{
		switch (par->current->type) {
		case TOKEN_IDENTIFIER: {
			ParseTree* iden = newTree(IDENTIFIER_PARSE, par->current);
			current->addChild(current, iden);
			break;
		}
		case TOKEN_NUMBER:
		case TOKEN_STRING:
		case TOKEN_TRUE:
		case TOKEN_FALSE: {
			ParseTree* value = newTree(ATOMIC_PARSE, par->current);
			current->addChild(current, value);
			break;
		}
		case TOKEN_PLUS:
		case TOKEN_MINUS:
		case TOKEN_STAR:
		case TOKEN_SLASH:
		case TOKEN_MODULO: {
			ParseTree* math = newTree(par->current->type, par->current);
			current->addChild(current, math);
			break;
		}
		case TOKEN_LEFT_PAREN: {
			ParseTree* paren = newTree(TOKEN_RIGHT_PAREN, par->current);
			current->addChild(current, paren);
			break;
		}
		case TOKEN_RIGHT_PAREN: {
			ParseTree* paren = newTree(TOKEN_RIGHT_PAREN, par->current);
			current->addChild(current, paren);
			break;
		}
		default:
			error(par, par->current, "Invalid Syntax");
			synchronize(par);
		}
		parserAdvance(par);
	}
	/*switch (par->current->type) {
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
						 error(par, par->current, "Invalid Syntax");
						 synchronize(par);
	}*/
	return 0;
}
// int x 5
// = 3 + 2
// int y = 10
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
		case TOKEN_STRING_V:
		case TOKEN_LOOP:
		case TOKEN_FUNCTION:
			parser->panic = 0;
			return;
		case TOKEN_END_LINE:
			parser->panic = 0;
			parserAdvance(parser);
			return;
		default:
			parserAdvance(parser);
		}
	}
}
void scanParser(Parser* par, ParseTree* current) {

	if (statement(par, current) && par->current->type == TOKEN_END_LINE) 
		parserAdvance(par);
		
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
		fprintf(stderr, " near '%.*s'", token->length, token->lexeme);
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
	switch (par->pre->type) {
	case TOKEN_INT_V: {
		mainTree->addChild(mainTree, newTree(PARSE_INT_V, par->pre));
		break;
	}
	case TOKEN_STRING_V: {
		mainTree->addChild(mainTree, newTree(PARSE_STRING_V, par->pre));
		break;
	}
	case TOKEN_FLOAT_V: {
		mainTree->addChild(mainTree, newTree(PARSE_FLOAT_V, par->pre));
		break;
	}
	default:
		error(par, par->current, "Unknown type");
		synchronize(par);
		return 0;
	}
	mainTree->addChild(mainTree, newTree(IDENTIFIER_PARSE, par->current));
	// int x = 5
	// int y += 10
	// int y = y + 10
	// y = 15
	parserAdvance(par);
	switch (par->current->type) {
	case TOKEN_EQUAL:
		mainTree->addChild(mainTree, newTree(PARSE_EQUAL, par->current));
		break;
	default:
		error(par, par->current, "Invalid Syntax");
		synchronize(par);
		return 0;
	}
	parserAdvance(par);
	ParseTree* treeExpression = newTree(EXPRESSION_PARSE, NULL);
	expression(par, treeExpression);
	mainTree->addChild(mainTree, treeExpression);
	
	current->addChild(current, mainTree);
	return 1;
}
