#include "../include/tooling.h"

void remove_spaces(char* s) {
    char* d = s;
    do {
        while (*d == ' ') {
            ++d;
        }
    } while (*s++ = *d++);
}

char* substr(char* s, int i, int l) {
    if (i + l > strlen(s) || i > strlen(s) || l > strlen(s)) return "";
    else {
        char* new = (char*)malloc(sizeof(char)*(l - i + 1));
        new = &s[i];
        new[l - i] = '\0';
        return new;
    }
}