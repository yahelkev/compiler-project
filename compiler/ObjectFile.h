#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "SymbolTable.h"

#define O_EXTENSION ".o"
#define COMPUTER 0x1c4
typedef struct ObjectFile
{
	char* filePath;
	FILE* filePointer;
	Table* table;

}ObjectFile;

void newObjectFile(ObjectFile* obj, char* path, Table* table);
void freeObjectFile(ObjectFile* obj);
FILE* CreateObjFile(const char* path);
void writeHeaders(ObjectFile* obj, int pcType, int numSections, int timeDate, int symbolTableOffset, int numSymbols, int SizeOptionalHeader, int flag);