#ifndef DICT_H
#define DICT_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tool.h"
#include "token.h"
#include "parser.h"
// Find a way to compare types when adding pairs to the dict
// typedef struct _value {
//     void* data;
//     char* type;
// } value;
// typedef struct _dict {
//     char** keys;
//     struct _value* values;
// } dict;
// struct _dict Dict(char* valuetype) {
//     struct _dict newDict;

// }

typedef int (*FUNCTION_EATER_POINTER)(Token* tk, Parser* par);

typedef struct _dict {
    int* _keys;
    FUNCTION_EATER_POINTER* _values;
    int _size;

    void (*destroy)(dict* di);
} dict;


struct _dict* Dict();
void __FREE_DICT(dict* di);
int has(struct _dict* dict, int key);
void add(struct _dict* dict, int key, FUNCTION_EATER_POINTER value);
FUNCTION_EATER_POINTER get(struct _dict* dict, int key);
void filedict(struct _dict* dict, char* filepath, char spacer, char del);

#endif // !DICT_H