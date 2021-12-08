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
		if (par->current->type != TOKEN_END_LINE) scanParser(par, par->mainTree);
		else parserAdvance(par);
	}
}

bool statement(Parser* par, ParseTree* current) {
	// Grammer rules and possiblities
	switch (par->current->type) {
	case TOKEN_INT_V:
	case TOKEN_FLOAT_V:
	case TOKEN_STRING_V:
		parserAdvance(par);
		return parseVariableCreation(par, current);
	case TOKEN_IF:
		parserAdvance(par);
		return parseConditional(par, current);
	default:
		error(par, par->current, "Invalid Syntax");
		synchronize(par);
	}
	return false;
}


bool expression(Parser* par, ParseTree* current, TokenType stopper) {
	// In the future will create a branch to explore this path
	// As of right now this will be very basic without arithmetic
	// Also will add function calls here when we get to that template
	


	switch (par->current->type) {
		case TOKEN_IDENTIFIER: {
			ParseTree* iden = newTree(IDENTIFIER_PARSE, par->current);
			current->addChild(current, iden);
			return true;
		}
		case TOKEN_NUMBER:
		case TOKEN_STRING: {
			ParseTree* iden = newTree(ATOMIC_PARSE, par->current);
			current->addChild(current, iden);
			return true;
		}
	}
	return false;
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
	par->pre = par->current;
	par->current = scanLexer(par->lex);

	if (par->current->type == TOKEN_ERROR) {
		error(par, par->pre, "Lexer error");
	}

}

bool parseVariableCreation(Parser* par, ParseTree* current) {
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
		error(par, par->pre, "Unknown type");
		synchronize(par);
		return false;
	}
	mainTree->addChild(mainTree, newTree(IDENTIFIER_PARSE, par->current));
	parserAdvance(par);
	switch (par->current->type) {
	case TOKEN_EQUAL:
		mainTree->addChild(mainTree, newTree(PARSE_EQUAL, par->current));
		break;
	default:
		error(par, par->pre, "Invalid Syntax");
		synchronize(par);
		return false;
	}
	parserAdvance(par);
	ParseTree* treeExpression = newTree(EXPRESSION_PARSE, NULL);
	expression(par, treeExpression, TOKEN_END_LINE);
	mainTree->addChild(mainTree, treeExpression);
	
	current->addChild(current, mainTree);
	parserAdvance(par);
	return true;
}

bool parseBody(Parser* par, ParseTree* current) {

	while (par->current->type == TOKEN_END_LINE) parserAdvance(par); // Skip space after closing paren
	if (par->current->type == TOKEN_LEFT_BRACE) {
		do { parserAdvance(par); } while (par->current->type == TOKEN_END_LINE);
		while (par->current->type != TOKEN_RIGHT_BRACE && par->current->type != TOKEN_EOF)
			scanParser(par, current);
		if (par->current->type == TOKEN_EOF) {
			error(par, par->pre, "Missing closing brace");
			return false;
		}
		parserAdvance(par);
		
	} else {
		while (par->current->type == TOKEN_END_LINE) parserAdvance(par);
		if (!statement(par, current)) return false;
	}
}

bool parseConditional(Parser* par, ParseTree* current) {
	
	ParseTree* mainTree = newTree(FULL_CONDITIONAL_PARSE, NULL);
	// Parse Condition

	ParseTree* condition = newTree(CONDITION_PARSE, NULL);
	ParseTree* treeExpression = newTree(EXPRESSION_PARSE, NULL);
	parserAdvance(par); // Remove when merged, only for tests
	if(!expression(par, treeExpression, TOKEN_RIGHT_PAREN)) return false; // Make if return
	parserAdvance(par); // Remove when merged, only for tests
	parserAdvance(par); // Remove when merged, only for tests
	condition->addChild(condition, treeExpression);
	mainTree->addChild(mainTree, condition);
	ParseTree* ifBody = newTree(IF_PARSE, NULL);
	parseBody(par, ifBody);
	mainTree->addChild(mainTree, ifBody);
	//do { parserAdvance(par); } while (par->current->type == TOKEN_END_LINE);
	//if (par->current->type != TOKEN_LEFT_BRACE) {
	//	while (par->current->type == TOKEN_END_LINE) parserAdvance(par);
	//	if (!statement(par, ifBody)) return false;
	//} else {
	//	do { parserAdvance(par); } while (par->current->type == TOKEN_END_LINE);
	//	while (par->current->type != TOKEN_RIGHT_BRACE && par->current->type != TOKEN_EOF)
	//		scanParser(par, ifBody);
	//	if (par->current->type == TOKEN_EOF) {
	//		error(par, par->pre, "Missing closing brace");
	//	}
	//}
	
	
	/*do { parserAdvance(par); } while (par->current->type == TOKEN_END_LINE);*/
	if (par->current->type == TOKEN_ELSE) {
		ParseTree* elseBody = newTree(ELSE_PARSE, NULL);
		parseBody(par, elseBody);
		mainTree->addChild(mainTree, elseBody);
		/*do { parserAdvance(par); } while (par->current->type == TOKEN_END_LINE);
		if (par->current->type != TOKEN_LEFT_BRACE) {
			while (par->current->type == TOKEN_END_LINE) parserAdvance(par);
			if (!statement(par, elseBody)) return false;
		} else {
			do { parserAdvance(par); } while (par->current->type == TOKEN_END_LINE);
			while (par->current->type != TOKEN_RIGHT_BRACE && par->current->type != TOKEN_EOF)
				scanParser(par, elseBody);
			if (par->current->type == TOKEN_EOF) {
				error(par, par->pre, "Missing closing brace");
			}
		}*/
	}

	current->addChild(current, mainTree);

	return true;
}
