#ifndef DICT_H
#define DICT_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tool.h"
#include "token.h"
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

typedef int (*FUNCTION_EATER_POINTER)(Token* tk);

typedef struct _dict {
    int* _keys;
    FUNCTION_EATER_POINTER* _values;
    int _size;
} dict;


struct _dict* Dict();
int has(struct _dict* dict, int key);
void add(struct _dict* dict, int key, FUNCTION_EATER_POINTER value);
FUNCTION_EATER_POINTER get(struct _dict* dict, int key);
void filedict(struct _dict* dict, char* filepath, char spacer, char del);

#endif // !DICT_H