#ifndef CODE_GEN_H
#define CODE_GEN_H

#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "ParseTree.h"
#include "SymbolTable.h"
#include "Postfix.h"
#include "StringList.h"
#include "LCConst.h"
#include "VariableHeap.h"
#include "FunctionDef.h"
#include "LoadBuiltin.h"

#define LENGTH(var) strlen(var) + 1
#define START_OF_FILE "\t.file "
#define ASM_EXTENSION ".asm"
#define MAX_DIGIT_LENGTH 15

#define ASCII_CAPITAL_A 'A'
#define ASCII_CAPITAL_Z 'Z'
#define ASCII_SMALL_A 'a'
#define ASCII_SMALL_Z 'z'

typedef enum {
	String_C,
	Code_C,
	Default_C,
}Literal;


typedef struct CodeGen {
	char* filePath;
	FILE* filePointer;
	Table* table;
	ParseTree* _main;

	LC_List* lcList;
	StringList* codeList;
	FunctionList* funcList;

	int loopCounter;
	int conditionCounter;

} CodeGen;

void newCodeGen(CodeGen* gen, char* path, ParseTree* mainTree, Table* table);
FILE* CreateBlankFile(char* path);
void Generate(CodeGen* gen, Heap_List* list, ParseTree* current, StringList* codeList);
void freeCodeGen(CodeGen* gen);
void emitAsm(CodeGen* gen);

char* getJmpCondition(ParseTreeType type);

// Valid cases
void CaseVariable(CodeGen* gen, Heap_List* heapList, ParseTree* current, StringList* codeList);
void CaseAssign(CodeGen* gen, Heap_List* heapList, ParseTree* current, StringList* codeList);
void CaseExpression(CodeGen* gen, Heap_List* heapList, ParseTree* current, StringList* codeList);
void CaseLoop(CodeGen* gen, Heap_List* heapList, ParseTree* current, StringList* codeList);
void CaseConditions(CodeGen* gen, Heap_List* heapList, ParseTree* current, StringList* codeList);
void CaseFunctionDef(CodeGen* gen, Heap_List* heapList, ParseTree* current, StringList* codeList);
void CaseFunctionCall(CodeGen* gen, Heap_List* heapList, ParseTree* current, StringList* codeList);

// Expression Handling
char* GetOPRow(CodeGen* gen, ParseTree* child, char* currentRow, StringList* codeList);
void PostToAsmExp(CodeGen* gen, Heap_List* heapList, ParseTree* child, StringList* codeList);
void ExpressionFirst(CodeGen* gen, Heap_List* heapList, ParseTree* child, StringList* codeList);

// Tooling
void writeLine(FILE* fp, const char* row);
char* assembleRow(char* asmRow, char* newRow); // Concats newRow to asmRow
void toLower(char* string);
#endif // !CODE_GEN_H


