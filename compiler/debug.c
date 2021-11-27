#include "Debug.h"



void printToken(Token* token) {
	if (token->type == TOKEN_ERROR) {
		printf("Error\t%d-%d\t%.*s\n", token->line, token->column, token->length, token->lexeme);
		return;
	}

	printf("%d\t%d-%d\t", token->type, token->line, token->column);
	printf("%d-%d\t", token->line, token->column + token->length - 1);

	if (token->type == TOKEN_IDENTIFIER || token->type == TOKEN_NUMBER || token->type == TOKEN_STRING) {
		printf("%.*s\n", token->length, token->lexeme);
	} else {
		char* keyword = findKeywordByType(token->type);

		if (keyword != NULL) {
			printf("%s\n", keyword);
		} else {
			printf("-\n");
		}
	}
}


void printTableValue(TABLE_VALUE* val) {
	switch (val->tag) {
		case FUNCTION_TAG:
			printf("FN << %s >>\t", val->function->returnType);
			for (size_t i = 0; i < val->function->amount; i++) {
				printf("%s (%s), ", val->function->args[i].type, val->function->args[i].name);
			}
			break;
		case VARIABLE_TAG:
			printf("VAR << %s >> [ %s ]\t", val->variable->type, val->variable->value);
			break;
		case ERROR_TAG:
			printf("error << %s >>\t", val->error->msg);
		default:
			return;
	}

	printf("%d-%d\n", val->line, val->column);
	return;
}