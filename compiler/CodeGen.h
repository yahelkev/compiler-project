#ifndef CODE_GEN_H
#define CODE_GEN_H

#include <stdlib.h>
#include <string.h>

#define LENGTH(var) strlen(var) + 1

typedef struct CodeGen {
	char* filePath;
} CodeGen;

void newCodeGen(CodeGen* gen, char* path);
#endif // !CODE_GEN_H
