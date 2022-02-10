#pragma once
#include "ObjectFile.h"


typedef struct Asseembler {
	Table* table;
	ObjectFile* _obj;
}Assembler;

void newAssembler(Assembler* asm, char* path, Table* table);
void freeAssembler(Assembler* asm);

void runAssembler(Assembler* asm);