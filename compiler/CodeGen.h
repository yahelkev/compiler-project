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

#define LENGTH(var) strlen(var) + 1
#define START_OF_FILE "\t.file "
#define ASM_EXTENSION ".asm"

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

	StringList* stringList;
	StringList* codeList;
} CodeGen;

void newCodeGen(CodeGen* gen, char* path, ParseTree* mainTree, Table* table);
FILE* CreateBlankFile(char* path);
void Generate(CodeGen* gen);
void emitByte(CodeGen* gen, Literal lit, const char* row);
void writeLine(FILE* fp, const char* row);

// Valid cases
void CaseVariable(const FILE* fp, ParseTree* current);
void CaseExpression(CodeGen* gen, ParseTree* tree);
#endif // !CODE_GEN_H
