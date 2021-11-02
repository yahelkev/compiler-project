#include "../inc/Lexer.h"

static void cleanLexer(Lexer* lex) {
        lex->source = NULL;
        lex->column = lex->line = 1;
        lex->index = lex->start = 0;
}

void newLexer(Lexer* lex, char* text) {
    cleanLexer(lex);
    lex->size = strlen(text);
    lex->text = (char*)malloc(sizeof( char ) * ( lex->size + 1 ));
    strcpy(lex->text, text);
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

Token MakeToken(Lexer* lex, TokenType type) {

    Token toke;

    toke.type = type;
    toke.length = 1;
    toke.lexeme = (char*)malloc( sizeof( char ) * ( toke.length + 1) );
    toke.lexeme[0] = lex->current;
    toke.lexeme[1] = '\0';
    toke.line = line;
    toke.column = column;

    return toke;
}

void advance(Lexer* lex, int stringFlag) {
    if((lex->current = peek(lex)) == '\n'  && !stringFlag) {
        lex->line += 1;
        lex->column = 1;
        lex->index += 1;
    } else if (lex->current != '\0') {
        lex->column += 1;
        lex->index += 1;
    }
    return;
}

Token makeErrorToken(Lexer* lexer, char* msg) {
	Token token;

	token.type = TOKEN_ERROR;
	token.lexeme = msg;
	token.length = strlen(msg);
	token.line = lexer->line;

	return token;
}

Token makeNumber(Lexer* lex) {
	while(isDigit(lex)) advance(lex);

	if (peek(lex) == '.') {
		advance(lex);
		while(isDigit(lex));
	}

	Token toke;

	toke.type = TOKEN_NUMBER;
    toke.length = lex->current - lex->start;
    toke.lexeme = (char*)malloc( sizeof( char ) * ( toke.length + 1) );
    strncpy( toke.lexeme, &lex->source[lex->start], toke.length );
	toke.line = lex->line;
    toke.column = lex->column;

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
    toke.length = lex->current - lex->start - 2;
    toke.lexeme = (char*)malloc( sizeof( char ) * ( toke.length + 1) );
    strncpy( toke.lexeme, &lex->source[lex->start + 1], toke.length );
	toke.line = lex->line;
    toke.column = lex->column;
    
	return toke;
}

Token scanLexer(Lexer* lex) {
    eatWhitespace(lex);

	lex->start = lex->index;

	if (isAtEnd(lex)) return makeToken(lex, TOKEN_EOF);

	if (isDigit(show(lex))) return makeNumber(lex);
	// if (isAlpha(show(lex))) return makeKeywordOrIdentifier(lex);

    switch( lex->current ) {
        case '(': return makeToken(lexer, TOKEN_LEFT_PAREN);
		case ')': return makeToken(lexer, TOKEN_RIGHT_PAREN);
		case '{': return makeToken(lexer, TOKEN_LEFT_BRACE);
		case '}': return makeToken(lexer, TOKEN_RIGHT_BRACE);
		case '[': return makeToken(lexer, TOKEN_LEFT_BRACKET);
		case ']': return makeToken(lexer, TOKEN_RIGHT_BRACKET);
		case ';': return makeToken(lexer, TOKEN_SEMICOLON);
		case ',': return makeToken(lexer, TOKEN_COMMA);

		case '+': return makeToken(lexer, match(lexer, '=') ? TOKEN_PLUS_EQUAL : match(lexer, '+') ? TOKEN_PLUS_PLUS: TOKEN_PLUS);
		case '-': return makeToken(lexer, match(lexer, '=') ? TOKEN_MINUS_EQUAL : match(lexer, '-') ? TOKEN_MINUS_MINUS: TOKEN_MINUS);
		case '*': return makeToken(lexer, match(lexer, '=') ? TOKEN_STAR_EQUAL : TOKEN_STAR);
		case '/': return makeToken(lexer, match(lexer, '=') ? TOKEN_SLASH_EQUAL : TOKEN_SLASH);
		case '%': return makeToken(lexer, match(lexer, '=') ? TOKEN_MODULO_EQUAL : TOKEN_MODULO);

		case '!': return makeToken(lexer, match(lexer, '=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
		case '=': return makeToken(lexer, match(lexer, '=') ? TOKEN_EQUAL_EQUAL : match(lexer, '>') ? TOKEN_EQUAL_GREATER: TOKEN_EQUAL);

		case '>': return makeToken(lexer, match(lexer, '=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
		case '<': return makeToken(lexer, match(lexer, '=') ? TOKEN_LESS_EQUAL : match(lexer, '|') ? TOKEN_LESS_OR: TOKEN_LESS);

        case '"':
		case '\'':
			return makeString(lexer, c);

		default:
			return makeErrorToken(lexer, "Unexpected token");
    }
}



