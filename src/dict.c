#include "../include/dict.h"

struct _dict* Dict() {
    struct _dict* newDict = (struct _dict*)malloc(sizeof(struct _dict));
    newDict->_keys = (int*)malloc(sizeof(int));
    newDict->_values = (FUNCTION_EATER_POINTER*)malloc(sizeof(FUNCTION_EATER_POINTER));
    newDict->_size = 0;
    return newDict;
}

void __FREE_DICT(dict* di) {
    free(di->_keys);
    for (size_t i = 0; i < di->_size; i++)
        free(di->_values[i]);
    free(di);
    return;
}

int has(struct _dict* dict, int key) {
    for (int i = 0; i < dict->_size; i++)
        if (dict->_keys[i] == key) return 1;
    return 0;
}
void add(struct _dict* dict, int key, FUNCTION_EATER_POINTER value) {
    if (has(dict, key)) {
        printf("dict error : key exists\n");
        exit(0);
    }
    dict->_keys = (int*)realloc(dict->_keys, sizeof(int) * (dict->_size + 1));
    dict->_values = (FUNCTION_EATER_POINTER*)realloc(dict->_values, sizeof(FUNCTION_EATER_POINTER) * (dict->_size + 1));
    dict->_keys[dict->_size] = key;
    dict->_values[dict->_size] = value;
    dict->_size++;
    return;
}
FUNCTION_EATER_POINTER get(struct _dict* dict, int key) {
    if (!dict->_size) {
        printf("dict error : dict empty\n");
        exit(0);
    }
    if (!has(dict, key)) {
        printf("dict error : key doesn't exists\n");
        exit(0);
    }
    for (int i = 0; i < dict->_size; i++)
        if (dict->_keys[i] == key) return dict->_values[i];
}