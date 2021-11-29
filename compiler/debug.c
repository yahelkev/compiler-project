#include "Debug.h"



void printToken(Token* token) {
	if (token->type == TOKEN_ERROR) {
		printf("Error\t%d-%d\t%.*s\n", token->line, token->column, token->length, token->lexeme);
		return;
	}

	printf("%d\t%d-%d\t", token->type, token->line, token->column);

	if (token->type == TOKEN_IDENTIFIER || token->type == TOKEN_NUMBER || token->type == TOKEN_STRING) {
		printf("%.*s\t", token->length, token->lexeme);
	} else {
		char* keyword = findKeywordByType(token->type);

		if (keyword != NULL) {
			printf("%s\t", keyword);
		} else {
			printf("-");
		}
	}

	printf("\t%d-%d\n", token->line, token->column+token->length);
}


void printTableValue(TABLE_VALUE* val) {
	switch (val->tag) {
		case FUNCTION_TAG:
			printf("<< %s >>\t", val->function->returnType);
			for (size_t i = 0; i < val->function->amount; i++) {
				printf("%s (%s), ", val->function->args[i].type, val->function->args[i].name);
			}
			break;
		case VARIABLE_TAG:
			printf("<< %s >> [ %s ]\t", val->variable->type, val->variable->value);
			break;
		case ERROR_TAG:
			printf("error << %s >>\t", val->error->msg);
		default:
			return;
	}

	printf("%d-%d\n", val->line, val->column);
	return;
}

void printParseTree(ParseTree* tree) {
	if (!tree) return;
	printf("Type: %d\n", tree->type);
	if (!tree->token) printToken(tree->token);
	printf("=> ");
	for (size_t i = 0; i < tree->amountOfChilds; i++) {
		printParseTree(tree->getChild(tree, i));
	}
}