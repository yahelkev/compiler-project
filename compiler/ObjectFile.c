#include "ObjectFile.h"

void newObjectFile(ObjectFile* obj, char* path) {
	obj->filePath = (char*)malloc(sizeof(char) * LENGTH(path));
	strncpy(obj->filePath, path, LENGTH(path));
	obj->filePointer = CreateObjFile(path);
	obj->fileHeaders = malloc(sizeof(fileHeader));
	obj->_textHeaders = malloc(sizeof(sectionHeadrer));
	strncpy(obj->_textHeaders->name, ".text", LENGTH(".text"));
	obj->_dataHeaders = malloc(sizeof(sectionHeadrer));
	strncpy(obj->_dataHeaders->name, ".data", LENGTH(".data"));
	obj->_bssHeaders = malloc(sizeof(sectionHeadrer));
	strncpy(obj->_bssHeaders->name, ".bss", LENGTH(".bss"));
	obj->_textSection = malloc(1);
	*obj->_textSection = '\0';
	obj->symbolTable = NULL;
	obj->relocationTable = NULL;
	obj->symbolTableSize = 0;
	obj->relocarionTableSize = 0;
}

void freeObjectFile(ObjectFile* obj) {
	free(obj->filePath);
	fclose(obj->filePointer);
	free(obj->fileHeaders);
	free(obj->_textHeaders);
	free(obj->_dataHeaders);
	free(obj->_bssHeaders);
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

void writeFile(ObjectFile* obj)
{
	fwrite(obj->fileHeaders, sizeof(fileHeader), 1, obj->filePointer);
	fwrite(obj->_textHeaders, sizeof(sectionHeadrer), 1, obj->filePointer);
	fwrite(obj->_dataHeaders, sizeof(sectionHeadrer), 1, obj->filePointer);
	fwrite(obj->_bssHeaders, sizeof(sectionHeadrer), 1, obj->filePointer);
	fwrite(obj->_textSection, sizeof(obj->_textSection), 1, obj->filePointer);
	for (int i = 0; i < obj->symbolTableSize; i++)
	{
		fwrite(obj->symbolTable[i], sizeof(symbosTableSection), 1, obj->filePointer);
	}
	for (int i = 0; i < obj->relocationTable; i++)
	{
		fwrite(obj->symbolTable[i], sizeof(relocationTableSection), 1, obj->filePointer);
	}

}

void setHeaders(ObjectFile* obj, int pcType, int numSections, int timeDate, int symbolTableOffset, int numSymbols, int SizeOptionalHeader, int flag)
{
	obj->fileHeaders->pcType = pcType;
	obj->fileHeaders->numSections = numSections;
	obj->fileHeaders->timedate = timeDate;
	obj->fileHeaders->symbolTablePtr = symbolTableOffset;
	obj->fileHeaders->numOfSymbols = numSymbols;
	obj->fileHeaders->OpHeaderSize = SizeOptionalHeader;
	obj->fileHeaders->flags = flag;
}

