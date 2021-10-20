#include "../include/parser.h"

int main(int argc, char** argv) {
    Parser* par = new_parser();
    Token* tk = new_token("i16", i16, 1, 1);
    tk->next = new_token("x", identifier, 1, 5);
    tk->next->next = new_token("=", assignment_op, 1, 7);
    tk->next->next = new_token("15", integer_literal, 1, 9);
    begin(tk, par);
    return 0;
}