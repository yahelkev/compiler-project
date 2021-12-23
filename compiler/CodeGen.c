#include "CodeGen.h"

void newCodeGen(CodeGen* gen, char* path) {
	gen->filePath = (char*)malloc(sizeof(char) * LENGTH(path));
	strncpy(gen->filePath, path, LENGTH(path));
	return;
}
