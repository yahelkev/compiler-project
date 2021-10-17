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

typedef struct _dict {
    int* _keys;
    int (*_values)(Token* tk);
    int _size;
} dict;


struct _dict* Dict();
int has(struct _dict* dict, char* key);
void add(struct _dict* dict, char* key, char* value);
char* get(struct _dict* dict, char* key);
void filedict(struct _dict* dict, char* filepath, char spacer, char del);

#endif // !DICT_H