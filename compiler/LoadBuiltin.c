#include "LoadBuiltin.h"


extern char* FunctionNames[FUNCTION_AMOUNT] = {
	"printi",
	"printf",
	"prints"
};

extern char* ArgTypes[FUNCTION_AMOUNT] = {
	"int",
	"float",
	"string"
};




void loadFunctionsToTable(Table* table) {
	for (size_t i = 0; i < FUNCTION_AMOUNT; i++) {
		insertValue(table, FunctionNames[i], newValue(FUNCTION_TAG, makeFunction(makeArg("x", ArgTypes[i]), 1, "void")));
	}
	return;
}

void loadFunctionBinaries(FunctionList* list) {

}
