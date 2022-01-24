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

#define LENGTH(var) strlen(var) + 1
#define START_OF_FILE "\t.file "
#define ASM_EXTENSION ".asm"
#define MAX_DIGIT_LENGTH 15

#define ASCII_CAPITAL_A 'A'
#define ASCII_CAPITAL_Z 'Z'
#define ASCII_SMALL_A 'a'
#define ASCII_SMALL_Z 'z'


typedef struct CodeGen {
	char* filePath; // Origin file path
	FILE* filePointer; // New .asm file pointer
	Table* table; // Symbol table pointer
	ParseTree* _main; // Main parse tree pointer

	LC_List* lcList; // List of all constants in the code
	StringList* codeList; // List of the entire asm code
	FunctionList* funcList; // List of function defentions & codelists for every function

	int loopCounter; // Counter for number of loops in code, entirley for asm purposes
	int conditionCounter; // Counter for number of loops in code, entirley for asm purposes

} CodeGen;


void newCodeGen(CodeGen* gen, char* path, ParseTree* mainTree, Table* table);
FILE* CreateBlankFile(char* path);
void Generate(CodeGen* gen, Heap_List* list, ParseTree* current, StringList* codeList); // Main generation for asm cases
void freeCodeGen(CodeGen* gen);
void emitAsm(CodeGen* gen); // Write entire code into file pointer

char* getJmpCondition(ParseTreeType type); // Returns reverse the conditional jmp op needed

// Valid cases
void CaseVariable(CodeGen* gen, Heap_List* heapList, ParseTree* current, StringList* codeList);
void CaseAssign(CodeGen* gen, Heap_List* heapList, ParseTree* current, StringList* codeList);
void CaseExpression(CodeGen* gen, Heap_List* heapList, ParseTree* current, StringList* codeList);
void CaseLoop(CodeGen* gen, Heap_List* heapList, ParseTree* current, StringList* codeList);
void CaseConditions(CodeGen* gen, Heap_List* heapList, ParseTree* current, StringList* codeList);
void CaseFunctionDef(CodeGen* gen, Heap_List* heapList, ParseTree* current, StringList* codeList);
void CaseFunctionCall(CodeGen* gen, Heap_List* heapList, ParseTree* current, StringList* codeList);

// Expression Handling
char* GetOPRow(CodeGen* gen, ParseTree* child, char* currentRow, StringList* codeList); // Returns the current math operation needed
void PostToAsmExp(CodeGen* gen, Heap_List* heapList, ParseTree* child, StringList* codeList); // Post fix to asm infix ish 
void ExpressionFirst(CodeGen* gen, Heap_List* heapList, ParseTree* child, StringList* codeList); // The first post fix to asm infix ish conversion, setts the first value into eax for further use

// Tooling
void writeLine(FILE* fp, const char* row); // Writes a new line to a file pointer given
char* assembleRow(char* asmRow, char* newRow); // Concats newRow to asmRow, and returns it
void toLower(char* string);
#endif // !CODE_GEN_H


