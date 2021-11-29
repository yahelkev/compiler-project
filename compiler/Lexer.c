#include "Lexer.h"

void freeToken(Token* toke) {
	if (!toke) return;
	free(toke->lexeme);
	free(toke);
}
void freeLexer(Lexer* lex) {
	if (!lex) return;
	free(lex->text);
	free(lex);
}

void cleanLexer(Lexer* lex) {
        lex->text = NULL;
        lex->column = lex->line = 1;
        lex->index = lex->start = 0;
}

void newLexer(Lexer* lex, char* text) {
    cleanLexer(lex);
    lex->size = strlen(text);
    lex->text = (char*)malloc(sizeof( char ) * ( lex->size + 1 ));
    strcpy(lex->text, text);
    lex->current = lex->text[lex->index];
    return;
}



char peek(Lexer* lex) {
    return lex->index + 1 == lex->size ? '\0' : lex->text[ lex->index + 1 ]; 
}

char show(Lexer* lex) {
    return lex->current;
}

bool isAtEnd(Lexer* lex) {
	return lex->current == '\0';
}

bool isDigit(char c) {
    return c >= '0' &&
           c <= '9';
}

bool isAlpha(char c) {
    return c >= 'a' && c <= 'z' ||
           c >= 'A' && c <= 'Z';
}

bool isIdentifier(char c) {
    return isAlpha(c) ||
           isDigit(c) ||
           c == '_';
}

bool match(Lexer* lex, char c) {
	if (show(lex) == c) {
		advance(lex);
		return true;
	}

	return false;
}

void eatWhiteSpace(Lexer* lex) {
    switch( lex->current ) {
		case ' ':
		case '\r':
		case '\n':
		case '\t':
			advance(lex);
			break;

		//comments
		case '/':
			if (show(lex) == '/') {
				while (advance(lex) != '\n' && !isAtEnd(lex));
				break;
			}

			if (show(lex) == '*') {
				advance(lex);
				advance(lex);
				while(show(lex) != '*' && show(lex) != '/') advance(lex);
				advance(lex);
				advance(lex);
				break;
			}

		default:
			return;
	}
    eatWhiteSpace(lex);
    return;
}

Token makeToken(Lexer* lex, TokenType type) {

    Token toke;

    toke.type = type;
    toke.length = 1;
    toke.lexeme = (char*)malloc( sizeof( char ) * ( toke.length + 1 ) );
    toke.lexeme[0] = lex->current;
    toke.lexeme[1] = '\0';
    toke.line = lex->line;
    toke.column = lex->column - 1;
	advance(lex);
    return toke;
}

char advance(Lexer* lex) {
    if((lex->current = peek(lex)) == '\n') {
        lex->line += 1;
        lex->column = 1;
        lex->index += 1;
    } else {
        lex->column += 1;
        lex->index += 1;
    }
    return lex->current;
}

Token makeErrorToken(Lexer* lex, char* msg) {
	Token toke;

	toke.type = TOKEN_ERROR;
	toke.lexeme = msg;
	toke.length = strlen(msg);
	toke.line = lex->line;
    toke.column = lex->column - 1;
	advance(lex);
	return toke;
}

Token makeNumber(Lexer* lex) {
	while(isDigit(show(lex))) advance(lex);

	if (peek(lex) == '.') {
		advance(lex);
		while(isDigit(show(lex))) advance(lex);
	}

	Token toke;

	toke.type = TOKEN_NUMBER;
    toke.length = lex->index - lex->start;
    toke.lexeme = (char*)malloc( sizeof( char ) * ( toke.length + 1) );
    strncpy( toke.lexeme, &lex->text[lex->start], toke.length );
    toke.lexeme[ toke.length ] = '\0';
	toke.line = lex->line;
    toke.column = lex->column - 1;

	return toke;
}

Token makeKeywordOrIdentifier(Lexer* lex) {
	advance(lex); //first letter can only be alpha

	while(isIdentifier(show(lex))) {
		advance(lex);
	}

	//scan for a keyword
	for (int i = 0; keywordTypes[i].keyword; i++) {
		if (strlen(keywordTypes[i].keyword) == lex->current - lex->start && !strncmp(keywordTypes[i].keyword, &lex->text[lex->start], lex->current - lex->start)) {
			Token toke;

			toke.type = keywordTypes[i].type;
			toke.length = lex->index - lex->start;
			toke.lexeme = (char*)malloc( sizeof( char ) * ( toke.length + 1) );
			strncpy( toke.lexeme, &lex->text[lex->start], toke.length );
			toke.lexeme[ toke.length ] = '\0';
			toke.line = lex->line;
			toke.column = lex->column - 1;

			return toke;
		}
	}

	//return an identifier
	Token toke;

	toke.type = TOKEN_IDENTIFIER;
	toke.length = lex->index - lex->start;
    toke.lexeme = (char*)malloc( sizeof( char ) * ( toke.length + 1) );
    strncpy( toke.lexeme, &lex->text[lex->start], toke.length );
    toke.lexeme[ toke.length ] = '\0';
	toke.line = lex->line;
    toke.column = lex->column - 1;

	return toke;
}

Token makeString(Lexer* lex, char terminator) {
	while (!isAtEnd(lex) && peek(lex) != terminator) {
		//escaping strings
		if (peek(lex) == '\\') {
			advance(lex);
		}

		advance(lex);
	}

	advance(lex); //eat terminator

	if (isAtEnd(lex)) {
		return makeErrorToken(lex, "Unterminated string");
	}


    Token toke;

	toke.type = TOKEN_STRING;
    toke.length = lex->index - lex->start - 2;
    toke.lexeme = (char*)malloc( sizeof( char ) * ( toke.length + 1) );
    strncpy( toke.lexeme, &lex->text[lex->start + 1], toke.length );
    toke.lexeme[ toke.length ] = '\0';
	toke.line = lex->line;
    toke.column = lex->column - 1;
    
	return toke;
}


Token scanLexer(Lexer* lex) {
    
    eatWhiteSpace(lex);
	lex->start = lex->index;
	if (isAtEnd(lex)) return makeToken(lex, TOKEN_EOF);

	if (isDigit(show(lex))) return makeNumber(lex);
	if (isAlpha(show(lex))) return makeKeywordOrIdentifier(lex);

    switch( lex->current ) {
        case '(': return makeToken(lex, TOKEN_LEFT_PAREN);
		case ')': return makeToken(lex, TOKEN_RIGHT_PAREN);
		case '{': return makeToken(lex, TOKEN_LEFT_BRACE);
		case '}': return makeToken(lex, TOKEN_RIGHT_BRACE);
		case '[': return makeToken(lex, TOKEN_LEFT_BRACKET);
		case ']': return makeToken(lex, TOKEN_RIGHT_BRACKET);
		case ';': return makeToken(lex, TOKEN_SEMICOLON);
		case ',': return makeToken(lex, TOKEN_COMMA);

		case '+': return makeToken(lex, match(lex, '=') ? TOKEN_PLUS_EQUAL : match(lex, '+') ? TOKEN_PLUS_PLUS: TOKEN_PLUS);
		case '-': return makeToken(lex, match(lex, '=') ? TOKEN_MINUS_EQUAL : match(lex, '-') ? TOKEN_MINUS_MINUS: match(lex,'>') ? TOKEN_MINUS_LESS : TOKEN_MINUS);
		case '*': return makeToken(lex, match(lex, '=') ? TOKEN_STAR_EQUAL : TOKEN_STAR);
		case '/': return makeToken(lex, match(lex, '=') ? TOKEN_SLASH_EQUAL : TOKEN_SLASH);
		case '%': return makeToken(lex, match(lex, '=') ? TOKEN_MODULO_EQUAL : TOKEN_MODULO);

		case '!': return makeToken(lex, match(lex, '=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
		case '=': return makeToken(lex, match(lex, '=') ? TOKEN_EQUAL_EQUAL : match(lex, '>') ? TOKEN_EQUAL_GREATER: TOKEN_EQUAL);

		case '>': return makeToken(lex, match(lex, '=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
		case '<': return makeToken(lex, match(lex, '=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);

        case '"':
		case '\'':
			return makeString(lex, lex->current);

		default:
			return makeErrorToken(lex, "Unexpected token");
    }
}



