#include "../include/parser.h"

Parser* new_parser() {
    Parser* par = (Parser*)malloc(sizeof(Parser));
    par->signatureList = Dict();
    par->destroy = &__FREE_PARSER;
    return par;
}

void __FREE_PARSER(Parser* par) {
    par->signatureList->destroy(par->signatureList);
    free(par);
    return;
}


void begin(Token* tk, Parser* par) {
    while (tk) {
        switch(tk->type) {
            case 16: {
                Token* head = tk;
                tk = tk->next;
                
            }

        }
    }
}