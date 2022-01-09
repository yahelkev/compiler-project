#ifndef CODE_GEN_H
#define CODE_GEN_H

#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <stdio.h>
#include <stdbool.h>

#include "ParseTree.h"
#include "SymbolTable.h"
#include "Postfix.h"
#include "StringList.h"
#include "LCConst.h"
#include "VariableHeap.h"

#define LENGTH(var) strlen(var) + 1
#define START_OF_FILE "\t.file "
#define ASM_EXTENSION ".asm"
#define MAX_DIGIT_LENGTH 15

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
	int loopCounter;
	int conditionCounter;
} CodeGen;

void newCodeGen(CodeGen* gen, char* path, ParseTree* mainTree, Table* table);
FILE* CreateBlankFile(char* path);
void Generate(CodeGen* gen, ParseTree* current);
void emitAsm(CodeGen* gen);
void Generate(CodeGen* gen, ParseTree* current);

char* getJmpCondition(ParseTreeType type);

// Valid cases
void CaseVariable(CodeGen* gen, Heap_List* heapList, ParseTree* current);
void CaseAssign(CodeGen* gen, Heap_List* heapList, ParseTree* current);
void CaseExpression(CodeGen* gen, Heap_List* heapList, ParseTree* current);
void CaseLoop(CodeGen* gen, Heap_List* heapList, ParseTree* current);
void CaseConditions(CodeGen* gen, Heap_List* heapList, ParseTree* current);


// Expression Handling
char* GetOPRow(CodeGen* gen, ParseTree* child, char* currentRow);
void PostToAsmExp(CodeGen* gen, Heap_List* heapList, ParseTree* child);
void ExpressionFirst(CodeGen* gen, Heap_List* heapList, ParseTree* child);

// Tooling
void writeLine(FILE* fp, const char* row);
char* assembleRow(char* asmRow, char* newRow); // Concats newRow to asmRow
#endif // !CODE_GEN_H
