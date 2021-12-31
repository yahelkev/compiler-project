#include "StringList.h"

StringList* newList() {
	StringList* list = (StringList*)malloc(sizeof(StringList));
	list->amount = 0;
	list->strings = (char**)malloc(sizeof(char*) * (list->amount + 1));
	return list;
}

void __ADD__(StringList* list, char* string) {
	list->strings = (char**)realloc(list->strings, sizeof(char*) * (list->amount + 1));
	list->strings[list->amount] = (char*)malloc(sizeof(char) * (LENGTH(string)));
	strncpy(list->strings[list->amount], string, strlen(string));
	list->strings[list->amount][strlen(string)] = '\0';
	return;
}