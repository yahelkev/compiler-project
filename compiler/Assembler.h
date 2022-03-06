#pragma once
#include <assert.h>
#include <stdio.h>

#include "ObjectFile.h"

#define BINARY_FILE_LIMIT_INT 10000
#define BINARY_FILE_LIMIT_STRING 5


typedef struct Asseembler {
	Table* table;
	ObjectFile* _obj;

	FILE* binaryCodeFilePointer; // File pointer given by the visitor, which points to the file which holds the binary code for all code ( main and functions )
}Assembler;

void newAssembler(Assembler* asm, char* path, Table* table);
void freeAssembler(Assembler* asm);

// --------------------

void runAssembler(Assembler* asm); // Function sets the values of objectFile, and manages the conversion of the asm code

// --------------------
// Tooling

int randInt(int n); // Returns an integer in the range [0, n)
