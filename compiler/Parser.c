#include "Parser.h"

void newParser(Parser* par, Lexer* lex) {
	par->lex = lex;
	par->pre = par->current = NULL;
	par->error = par->panic = 0;
	par->mainTree = newTree(MAIN_PARSE, NULL);
	par->table = (Table*)malloc(sizeof(Table));
	newTable(par->table);
	parserAdvance(par);
}

void startParsing(Parser* par) {
	synchronize(par);
	while(par->current->type != TOKEN_EOF) {
		if (par->current->type != TOKEN_END_LINE) scanParser(par, par->mainTree);
		else parserAdvance(par);
	}
}

bool statement(Parser* par, ParseTree* current) {


	if (par->current->type == TOKEN_IDENTIFIER) {
		parserAdvance(par);
		return parseAssign(par, current);
	}


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
	case TOKEN_FUNCTION:
		parserAdvance(par);
		return parseFunction(par, current);
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

	//synchronize(par);
	if (par->current->type != TOKEN_EOF && 
		statement(par, current) && 
		par->current->type == TOKEN_END_LINE)
		parserAdvance(par);
		
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
	if (par->current->type != TOKEN_IDENTIFIER) {
		error(par, par->current, "Missing identifier");
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
	if (isDefined(par->table, mainTree->getChild(mainTree, START_TREE + 1)->token->lexeme)) {
		error(par, mainTree->getChild(mainTree, START_TREE + 1)->token, "Variable already initialized");
		mainTree->freeParseTree(mainTree);
		synchronize(par);
		return false;
	}
	current->addChild(current, mainTree);
	struct variable var = makeVariable(mainTree->getChild(mainTree, START_TREE), mainTree->getChild(mainTree, END_VARIABLE_TREE));
	TABLE_VALUE* value = (TABLE_VALUE*)malloc(sizeof(TABLE_VALUE));
	newValue(value, VARIABLE_TAG, &var, mainTree->getChild(mainTree, START_TREE)->token->line, mainTree->getChild(mainTree, START_TREE)->token->column);
	insertValue(par->table, mainTree->getChild(mainTree, START_TREE + 1)->token->lexeme, *value);
	parserAdvance(par);
	return true;
}



bool parseAssign(Parser * par, ParseTree * current) {
	ParseTree* mainTree = newTree(ASSIGN_PARSE, NULL);
	if (!isDefined(par->table, par->pre->lexeme)) {
		error(par, par->pre, "Variable used but not defined");
		mainTree->freeParseTree(mainTree);
		synchronize(par);
		return false;
	}
	TABLE_VALUE val = getValue(par->table, par->pre->lexeme);
	if (val.tag == FUNCTION_TAG) {
		error(par, par->pre, "Function type is unassignable");
		mainTree->freeParseTree(mainTree);
		synchronize(par);
		return false;
	}

	mainTree->addChild(mainTree, newTree(IDENTIFIER_PARSE, par->pre));
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
			mainTree->freeParseTree(mainTree);
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
	if(!parseBody(par, ifBody)) return false;
	mainTree->addChild(mainTree, ifBody);
	
	if (par->current->type == TOKEN_ELSE) {
		ParseTree* elseBody = newTree(ELSE_PARSE, NULL);
		if (!parseBody(par, elseBody)) return false;
		mainTree->addChild(mainTree, elseBody);
	}
	current->addChild(current, mainTree);
	return true;
}


bool parseFunction(Parser* par, ParseTree* current) {
	if (par->current->type != TOKEN_IDENTIFIER) {
		// Might make this for anonymous function declarations in the future
		error(par, par->pre, "Missing function name");
		synchronize(par);
		return false;
	}
	if (isDefined(par->table, par->current->lexeme)) {
		error(par, par->current, "Function already initialized");
		synchronize(par);
		return false;
	}
	ParseTree* mainTree = newTree(FULL_FUNCTION_PARSE, NULL);
	mainTree->addChild(mainTree, newTree(FUNCTION_NAME_PARSE, par->current));
	parserAdvance(par);
	ParseTree* args = newTree(FUNCTION_ARGS_PARSE, NULL);
	if (!parseArgs(par, args)) return false;
	
	
	mainTree->addChild(mainTree, args);
	switch (par->current->type) {
		default:
			error(par, par->pre, "Unknown type or missing type");
			synchronize(par);
			return false;
		case TOKEN_INT_V:
		case TOKEN_STRING_V:
		case TOKEN_FLOAT_V:
			parserAdvance(par);
			break;
	}
	ParseTree* type = newTree(FUNCTION_TYPE_PARSE, par->pre);
	mainTree->addChild(mainTree, type);
	ParseTree* body = newTree(FUNCTION_BODY_PARSE, NULL);
	if(!parseBody(par, body)) return false;
	mainTree->addChild(mainTree, body);
	current->addChild(current, mainTree);

	
	// Add function to symbol table
	struct arg* args_s = (struct arg*)malloc(sizeof(struct arg));
	size_t i = 0;
	ParseTree* argTree = args->getChild(args, i);
	for (i = 0; i < args->amountOfChilds; i++, argTree = args->getChild(args, i)) {
		args_s = (struct atg*)realloc(args, sizeof(struct arg) * (i + 1));
		args_s[i] = makeArg(argTree->getChild(argTree, START_TREE + 1)->token->lexeme, argTree->getChild(argTree, START_TREE)->token->lexeme);
	}
	TABLE_VALUE* value = (TABLE_VALUE*)malloc(sizeof(TABLE_VALUE));
	struct function func = makeFunction(args_s, i, type->token->lexeme);
	newValue(value, FUNCTION_TAG, &func, argTree->getChild(argTree, START_TREE)->token->line, argTree->getChild(argTree, START_TREE)->token->column);
	insertValue(par->table, argTree->getChild(argTree, START_TREE + 1)->token->lexeme, *value);
	return true;
}

bool parseArgs(Parser* par, ParseTree* current) {
	/** 3 Types of args
		()
		(arg1)
		(arg1, arg2,...)
	*/
	parserAdvance(par);
	if (par->pre->type != TOKEN_LEFT_PAREN) {
		error(par, par->pre, "Missing '(' at function declaration");
		synchronize(par);
		return false;
	}
	if (par->current->type == TOKEN_RIGHT_PAREN) {
		parserAdvance(par);
		return true;
	}
	
	do {
		parserAdvance(par);
		// TODO : Make args in symbol table
		ParseTree* argTree = newTree(ARG_PARSE, NULL);
		switch (par->pre->type) {
		case TOKEN_INT_V: {
			argTree->addChild(argTree, newTree(PARSE_INT_V, par->pre));
			break;
		}
		case TOKEN_STRING_V: {
			argTree->addChild(argTree, newTree(PARSE_STRING_V, par->pre));
			break;
		}
		case TOKEN_FLOAT_V: {
			argTree->addChild(argTree, newTree(PARSE_FLOAT_V, par->pre));
			break;
		}
		default:
			error(par, par->pre, "Unknown type");
			synchronize(par);
			return false;
		}

		if (par->current->type != TOKEN_IDENTIFIER) {
			error(par, par->current, "Missing identifier");
			synchronize(par);
			return false;
		}
		argTree->addChild(argTree, newTree(IDENTIFIER_PARSE, par->current));
		
		current->addChild(current, argTree);
		parserAdvance(par);
		parserAdvance(par);
	} while (par->pre->type == TOKEN_COMMA);
	if (par->pre->type != TOKEN_RIGHT_PAREN) {
		error(par, par->pre, "Missing ')'");
		synchronize(par);
		return false;
	}
	//parserAdvance(par);
	return true;
}


