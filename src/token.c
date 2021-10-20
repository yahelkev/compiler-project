#include "../include/token.h"

Token* new_token(char* value, int type, int line, int column) {
    Token* new = (Token*)malloc(sizeof(Token));
    new->value = (char*)malloc(sizeof(char)*(strlen(value)+1));
    strcpy(new->value, value);     
    new->type = type;
    new->line = line;
    new->column = column;
    new->destroy = &__FREE_TOKEN__;
    new->next = NULL;
    return new;
}

void __FREE_TOKEN__(Token* tk) {
    free(tk->value);
    free(tk);
    return;
}