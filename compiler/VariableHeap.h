#ifndef VARIABLE_HEAP_H
#define VARIABLE_HEAP_H

#include <stdlib.h>
#include <string.h>

#define LENGTH(var) strlen(var) + 1
#define DWORD_SIZE 4

typedef enum {
	HEAP_DWORD,
}Heap_TYPE;


// An enum to know which margin type we are searching for in the heap list
typedef enum {
	SEARCH_VARIABLE,
	SEARCH_ARG
}Search_Flag;


// Struct to keep track of local variables that are kept on the stack, stores the type and name of the variable/arg and also its margin on the stack from the current rbp
typedef struct VariableHeap {
	Heap_TYPE type;
	char* key; // i.e. name of the variable/arg
	int margin; // i.e. amount of spots you move in the stack, will be in jumps of 4, positive for args and negative for local variables
}VariableHeap;

// Strcut to keep track of list of all VariableHeap structures
typedef struct Heap_List {
	VariableHeap** heaps;
	int size;
}Heap_List;


VariableHeap* getHeap(Heap_List* list, char* key); // Returns heap based on variable name
int getLast(Heap_List* list, Search_Flag flag); // Returns latest heap margin based on heap type, plus new margin


VariableHeap* newHeap(Heap_TYPE type, char* key, int margin); // Creates a new heap
Heap_List* newHeap_List(); // Creates a new heap listt
void Heap_ListAdd(Heap_List* list, VariableHeap* heap); // Adds new heap to heap list
void Heap_ListDel(Heap_List* list, int index); // Delets and frees heap at index from list
void Heap_ListDelLast(Heap_List* list, int amount); // Deletes and frees n last heaps from heap list
void freeHeap_List(Heap_List* list); // Frees heap listt
void freeHeap(VariableHeap* heap); // Frees heap

#endif // !VARIABLE_HEAP_H
