#include "../include/dict.h"

struct _dict* Dict() {
    struct _dict* newDict = (struct _dict*)malloc(sizeof(struct _dict));
    newDict->_keys = (int*)malloc(sizeof(int));
    newDict->_values = (FUNCTION_EATER_POINTER*)malloc(sizeof(FUNCTION_EATER_POINTER));
    newDict->_size = 0;
    return newDict;
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
    dict->_values[dict->_size] = &value;
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

void filedict(struct _dict* dict, char* filepath, char spacer, char del) {
    char* buffer = readFile(filepath);
    int bufferSize = strlen(buffer), currentCopy = 0, keySize = 1, valueSize = 1; // 0 for key, 1 for value
    char* currentKey = (char*)malloc(sizeof(char)), * currentValue = (char*)malloc(sizeof(char));
    currentKey[0] = '\0';
    currentValue[0] = '\0';
    for (int i = 0; i < bufferSize; i++) {
        if (buffer[i] == spacer) {
            currentCopy = !currentCopy;
        } else if (buffer[i] == del) {
            add(dict, currentKey, currentValue);
            currentKey = (char*)realloc(currentKey, sizeof(char));
            currentValue = (char*)realloc(currentValue, sizeof(char));
            keySize = valueSize = 1;
            currentCopy = !currentCopy;
        } else if (!currentCopy) {
            currentKey = (char*)realloc(currentKey, sizeof(char) * (++keySize));
            currentKey[keySize - 2] = buffer[i];
            currentKey[keySize - 1] = '\0';
        } else if (currentCopy) {
            currentValue = (char*)realloc(currentValue, sizeof(char) * (++valueSize));
            currentValue[valueSize - 2] = buffer[i];
            currentValue[valueSize - 1] = '\0';
        }
    }
    add(dict, currentKey, currentValue);
}