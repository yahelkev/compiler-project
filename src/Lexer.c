#include "../inc/Lexer.h"


void newLexer(Lexer* lex, char* text) {
    lex->column = lex->line = 1;
     lex->index = 0;
    lex->size = strlen(text);
    lex->text = (char*)malloc(sizeof( char ) * ( size + 1 ));
    strcpy(lex->text, text);
    return;
}

char peek(Lexer* lex) {
    return lex->index + 1 == lex->size ? '\0' : lex->text[ lex->index + 1 ]; 
}

char show(Lexer* lex) {
    return lex->current;
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