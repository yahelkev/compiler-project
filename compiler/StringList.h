#ifndef STRING_LIST_H
#define STRING_LIST_H

typedef struct StringList {
	char** strings;
	int amount;

	void (*add)(struct StringList*, char*);
}StringList;

StringList* newList();
void __ADD__(StringList* list, char* string);
#endif // !STRING_LIST_H


