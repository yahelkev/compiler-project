#include "keyword_types.h"

KeywordType keywordTypes[] = {
	{"else", TOKEN_ELSE},
	{"false", TOKEN_FALSE},
	{"for", TOKEN_FOR},
	{"if", TOKEN_IF},
	{"print", TOKEN_PRINT},
	{"return", TOKEN_RETURN},
	{"fn", TOKEN_FUNC},
	{"true", TOKEN_TRUE},
	{"while", TOKEN_WHILE},

	{NULL, TOKEN_EOF}
};

char* findKeywordByType(TokenType type) {
	for(int i = 0; keywordTypes[i].keyword; i++) {
		if (keywordTypes[i].type == type) {
			return keywordTypes[i].keyword;
		}
	}

	return NULL;
}