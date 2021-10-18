#ifndef TOKEN_H
#define TOKEN_H
#include <stdlib.h>
#include <string.h>
enum TYPE {
    i16,
    i32,
    f16,
    f32,
    identifier,
    integer_literal,
    assignment_op,
    semi_colon
};
typedef struct TOKEN {
    int line, column, type;
    char* value;
    struct TOKEN* next;

    void (*destroy)(Token*);
}Token;

Token* new_token(char*, int, int, int);
void __FREE_TOKEN__(Token*);

#endif // !TOKEN_H