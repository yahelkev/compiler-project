#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "SymbolTable.h"

#define O_EXTENSION ".o"

typedef struct ObjectFile
{
	char* filePath;
	FILE* filePointer;
	Table* table;

}ObjectFile;

void newObjectFile(ObjectFile* obj, char* path, Table* table);
void freeObjectFile(ObjectFile* obj);
FILE* CreateObjFile(const char* path);