#include "../include/parser.h"
Parser* new_parser() {
    Parser* par = (Parser*)malloc(sizeof(Parser));
    par->signatureList = Dict();
    par->destroy = &__FREE_PARSER;
    return par;
}

Call* new_call(Token* tk, int check) {
    Call* cal = (Call*)malloc(sizeof(Call));
    cal->tk = tk;
    cal->check = check;
    return cal;
}

void __FREE_PARSER(Parser* par) {
    par->signatureList->destroy(par->signatureList);
    free(par);
    return;
}

Call* eat_i16(Token* tk, Parser* par) {
    Call* cal = new_call(tk, 1);// tk -> head
    tk = tk->next;
    if(tk->type == identifier)
        tk = tk->next;
    else
        return cal;
    if(tk->type == assignment_op)
        tk = tk->next;
    else
        return cal;
    if(tk->type == integer_literal)
        tk = tk->next;
    else
        return cal;
    cal->tk = tk;
    cal->check = 0;
    return cal;
}

void begin(Token* tk, Parser* par) {
    Call* cal = new_call(NULL, 1);
    while (tk) {
        cal->check = 1;
        switch(tk->type) {
            case i16: {
                Token* head = tk;
                if((cal = eat_i16(tk, par))->check) {
                    tk = head;
                } else {
                    printf("parsed tree succesfully\n");
                    tk = cal->tk;
                }
                break;
            }

        }
        if(cal->check) {
            printf("error while parsing tree\n");
            break;
        }
    }
}