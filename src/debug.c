#include "../inc/Debug.h"



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
			printf("%s", keyword);
		} else {
			printf("-");
		}
	}

	printf("\t%d-%d\n", token->line, token->column);
}