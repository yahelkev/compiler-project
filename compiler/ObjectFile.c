#include "ObjectFile.h"

void newObjectFile(ObjectFile* obj, char* path, Table* table) {
	obj->filePath = (char*)malloc(sizeof(char) * LENGTH(path));
	strncpy(obj->filePath, path, LENGTH(path));
	obj->table = table;
	obj->filePointer = CreateObjFile(path);
}

void freeObjectFile(ObjectFile* obj) {
	free(obj->filePath);
	fclose(obj->filePointer);
}

FILE* CreateObjFile(const char* path) {
	FILE* fp;
	char* objPath = (char*)malloc(sizeof(char) * LENGTH(path));
	strncpy(objPath, path, LENGTH(path));
	int pos = 0;
	for (size_t i = 0; i < strlen(objPath); i++)
		if (objPath[i] == '.') pos = i;
	objPath[pos] = '\0';
	objPath = (char*)realloc(objPath, sizeof(char) * LENGTH(objPath) + LENGTH(O_EXTENSION));
	strncat(objPath, O_EXTENSION, LENGTH(O_EXTENSION));

	fp = fopen(objPath, "wb");
	return fp;
}

void writeHeaders(ObjectFile* obj, int pcType, int numSections, int timeDate, int symbolTableOffset, int numSymbols, int SizeOptionalHeader, int flag)
{
	fwrite((const void*)&pcType, 2, 1, obj->filePointer);
	fwrite((const void*)&numSections, 2, 1, obj->filePointer);
	fwrite((const void*)&timeDate, 4, 1, obj->filePointer);
	fwrite((const void*)&symbolTableOffset, 4, 1, obj->filePointer);
	fwrite((const void*)&numSymbols, 4, 1, obj->filePointer);
	fwrite((const void*)&SizeOptionalHeader, 2, 1, obj->filePointer);
	fwrite((const void*)&flag, 2, 1, obj->filePointer);
}

