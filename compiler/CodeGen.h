#ifndef CODE_GEN_H
#define CODE_GEN_H

#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <stdio.h>

#define LENGTH(var) strlen(var) + 1
#define START_OF_FILE "\t.file "
#define ASM_EXTENSION ".asm"

typedef struct CodeGen {
	char* filePath;
	FILE* filePointer;
} CodeGen;

void newCodeGen(CodeGen* gen, char* path);
FILE* CreateBlankFile(char* path);
#endif // !CODE_GEN_H
