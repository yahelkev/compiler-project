#ifndef LOAD_BUILTIN_H
#define LOAD_BUILTIN_H

#include "SymbolTable.h"
#include "FunctionDef.h"

// I want to add an option to load all of the builtin functions through dedicated files
// For now we will leave with just normal strings, and constant data
// It ain't pretty but it will get the job done

typedef enum NAMES{
	PRINT_INT,
	PRINT_FLOAT,
	PRINT_STRING,
	FUNCTION_AMOUNT,
}NAMES;

extern char* FunctionNames[];
extern char* ArgTypes[]; // This works on the idea that each function has exactly one arg, again this is very ugly, but will work for now
void loadFunctionsToTable(Table* table);
void loadFunctionBinaries(FunctionList* list);

#endif // !LOAD_BUILTIN_H
