#include "Visitor.h"

Error_Codes newVisitor(Visitor* vis, Parser* par) {
	//vis = (Visitor*)malloc(sizeof(Visitor));
	if (vis == NULL) return VIS_MALLOC_FAIL;
	if (par->error) return VIS_PARSER_ERROR;
	
	vis->par = par;
	return VIS_OK;
}
void throwError(Token* token, const char* message) {
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
}
void visitAll(Visitor* vis) {
	while (getParseTree(vis->par)) {
		// Check current ast
		// Returns most recent parseTree structure
		visitAst(vis->par->table, vis->par->mainTree->getChild(vis->par->mainTree, vis->par->mainTree->amountOfChilds - 1));
	}
}

bool visitAst(Table* table, ParseTree* tree) {
	// Works close to how the statement funciton worked in the parser, as checking current tree and going down in levels if necessary
	switch (tree->type) {
	case FULL_CALL_PARSE:
		return visitCall(table, tree);
	case EXPRESSION_PARSE:
		return visitExperssion(table, tree);
	default:
		return false;
	}
}

bool visitCall(Table* table, ParseTree* tree) {
	// Check wheter call tree is ok or not with args
	ParseTree* argsTree = tree->getChild(tree, 1);
	TABLE_VALUE val = getValue(table, tree->getChild(tree, 0)->token->lexeme);
	
	if (argsTree->amountOfChilds != val.function->amount) {
		// Args amount don't match
		throwError(tree->getChild(tree, 0)->token, "Function parameters amount mismatched");
		return false;
	}

	ParseTree* currentArg = argsTree->getChild(argsTree, 0);
	bool return_ = true;
	for (size_t i = 0; i < argsTree->amountOfChilds; i++, currentArg = argsTree->getChild(argsTree, i)) {
		if (strcmp(getTypeOfExpression(table, currentArg),val.function->args[i].type)) {
			throwError(currentArg->getChild(currentArg, 0)->token, "Type mismatch on function call");
			return_ = false;
		}
	}
	return return_;


}

bool visitExperssion(Table* table, ParseTree* tree)
{
	ParseTree* child = tree->getChild(tree, 0);
	ParseTreeType type = getTypeAsParseType(table, child);
	for (int i = 1; i < tree->amountOfChilds; i++)
	{
		ParseTree* child = tree->getChild(tree, i);
		if (child->type == ATOMIC_PARSE || child->type == PARSE_STRING)
		{
			if (getTypeAsParseType(table, child) != type) {
				throwError(child->token, "Make sure it is the same type");
				return false;
			}
		}
	}
	return true;
}

char* getTypeOfExpression(Table* table, ParseTree* tree) {
	ParseTree* first = tree->getChild(tree, 0);
	switch (first->type) {
	case IDENTIFIER_PARSE:
		return getValue(table, first->token->lexeme).variable->type;
	case ATOMIC_PARSE: {
		switch (first->token->type) {
		case TOKEN_INT:
			return "int";
		case TOKEN_FLOAT:
			return "float";
		}
		break;
	}
		// Got to find some kind of check
		return "int";
	case FULL_CALL_PARSE:
		return getValue(table, first->getChild(first, 0)->token->lexeme).function->returnType;
	}
	return "void";
}

ParseTreeType getTypeAsParseType(Table* table, ParseTree* child)
{
	if (child->type == IDENTIFIER_PARSE)
	{
		if (strcmp(getValue(table, child->token->lexeme).variable->type, "string"))
		{
			return PARSE_STRING;
		}
		else if (strcmp(getValue(table, child->token->lexeme).variable->type, "float") || strcmp(getValue(table, child->token->lexeme).variable->type, "int"))
		{
			return ATOMIC_PARSE;
		}
	}
	return child->type;
}

bool getParseTree(Parser* par) {
	synchronize(par);
	if (par->current->type != TOKEN_EOF) {
		if (par->current->type != TOKEN_END_LINE) scanParser(par, par->mainTree);
		else parserAdvance(par);
		return true;
	}
	return false;
}
