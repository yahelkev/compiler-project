#pragma once
#include "ObjectFile.h"


typedef struct Asseembler {
	Table* table;
	ObjectFile* _obj;
}Assembler;

void newAssembler(Assembler* asm, char* path, Table* table);
void freeAssembler(Assembler* asm);
//function sets the valuse of objectFile and manages the conversion of the assembly code
void runAssembler(Assembler* asm);