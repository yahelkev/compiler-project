#include "CodeGen.h"

void newCodeGen(CodeGen* gen, char* path, ParseTree* mainTree, Table* table) {
	gen->filePath = (char*)malloc(sizeof(char) * LENGTH(path));
	strncpy(gen->filePath, path, LENGTH(path));
	gen->_main = mainTree;
	gen->table = table;
	gen->codeList = newStringList();
	gen->lcList = newLC_List();
	return;
}

void freeCodeGen(CodeGen* gen) {
	gen->_main->freeParseTree(gen->_main);
	free(gen->filePath);
	fclose(gen->filePointer);
	free(gen);
	return;
}

FILE* CreateBlankFile(const char* path) {
	FILE* fp;
	char* asmPath = (char*)malloc(sizeof(char) * LENGTH(path));
	strncpy(asmPath, path, LENGTH(path));
	int pos = 0;
	for (size_t i = 0; i < strlen(asmPath); i++)
		if (asmPath[i] == '.') pos = i;
	asmPath[pos] = '\0';
	asmPath = (char*)realloc(asmPath, sizeof(char) * LENGTH(asmPath) + LENGTH(ASM_EXTENSION));
	strncat(asmPath, ASM_EXTENSION, LENGTH(ASM_EXTENSION));

	fp = fopen(asmPath, "w");
	char* btext = (char*)malloc(sizeof(char) * LENGTH(START_OF_FILE));
	strncpy(btext, START_OF_FILE, LENGTH(START_OF_FILE));
	btext = (char*)realloc(btext, sizeof(char) * LENGTH(btext) + strlen(path));
	strncat(btext, path, LENGTH(path));
	fwrite(btext, sizeof(char), strlen(btext), fp);
	return fp;
}

void Generate(CodeGen* gen) {
	size_t index = 0;
	ParseTree* currentChild = gen->_main->getChild(gen->_main, index);
	Heap_List* heapList = newHeap_List();
	for (index = 0; index < gen->_main->amountOfChilds; index++, currentChild = gen->_main->getChild(gen->_main, index)) {

		switch (currentChild->type) {
		case VARIABLE_PARSE:
			CaseVariable(gen, heapList, currentChild);
			break;
		}
	}
}

void CaseExpression(CodeGen* gen, Heap_List* heapList, ParseTree* current) {
	size_t i = 0;
	ParseTree* child = current->getChild(current, i);
	for (; i < current->amountOfChilds; i++, child = current->getChild(current, i)) {
        if (child->type == IDENTIFIER_PARSE){
			char* currentRow = (char*)malloc(1);
			*currentRow = '\0';
			char marginString[15] = "";
			assembleRow(currentRow, "PUSH  DWORD PTR [rbp-");
			sprintf(marginString, "%d", getHeap(heapList, child->token->lexeme)->margin);
			assembleRow(currentRow, marginString);
			assembleRow(currentRow, "]");
			gen->codeList->add(gen->codeList, currentRow);
        }
		else if(child->type == ATOMIC_PARSE) {
			char* currentRow = (char*)malloc(1);
			*currentRow = '\0';
			char numSTR[15] = "";
			switch (child->token->type)
			{
			case TOKEN_STRING:
					assembleRow(currentRow, "PUSH	OFFSET FLAT:.LC");
					sprintf(numSTR, "%d", get_LC_offset(gen->lcList, child->token->lexeme));
					assembleRow(currentRow, numSTR);
					*numSTR = '\0';
				break;
			case TOKEN_FLOAT:
					assembleRow(currentRow, "PUSH	DWORD PTR .LC");
					sprintf(numSTR, "%d",get_LC_offset(gen->lcList, child->token->lexeme));
					assembleRow(currentRow, numSTR);
					*numSTR = '\0';
					assembleRow(currentRow, "[rip]");
				break;
			case TOKEN_INT:
					assembleRow(currentRow, "PUSH	");
					assembleRow(currentRow, child->token->lexeme);
				break;
			default:
				break;
			}
			gen->codeList->add(gen->codeList, currentRow);
		}
        else
        {
			gen->codeList->add(gen->codeList, "POP	edx");
			gen->codeList->add(gen->codeList, "POP	eax");
            switch (child->type)
            {
			case PARSE_PLUS:
            {
				gen->codeList->add(gen->codeList, "add	eax, edx");
                break;
            }
            case PARSE_MINUS:
            {
				gen->codeList->add(gen->codeList, "sub	eax, edx");
                break;
            }
            case PARSE_STAR:
            {
				gen->codeList->add(gen->codeList, "imul	eax, edx");
                break;
            }
            case PARSE_SLASH:
            {
				gen->codeList->add(gen->codeList, "cdq");
				gen->codeList->add(gen->codeList, "idiv	edx");
                break;
            }
            }
			gen->codeList->add(gen->codeList, "PUSH	eax");
        }
    }
	gen->codeList->add(gen->codeList, "POP	eax");
}


void CaseVariable(CodeGen* gen, Heap_List* heapList , ParseTree* current) {

	switch (current->getChild(current, 0)->type) {
		case PARSE_INT_V: {
			// Parsing the value of the variable
			CaseExpression(gen, heapList, current->getChild(current, 3));

			// making the actuall row of creating the varialbe
			char* currentRow = NULL;
			int newMargin = heapList->size > 0 ? heapList->heaps[heapList->size - 1]->margin + 4 : 4;
			Heap_ListAdd(heapList, newHeap(HEAP_DWORD, current->getChild(current, 1)->token->lexeme, newMargin));
			assembleRow(currentRow, "PUSH DWORD PTR [rbp-");
			char* marginString= (char*)malloc((int)((ceil(log10((int)newMargin)) + 1) * sizeof(char)));
			sprintf(marginString, "%d", newMargin);
			assembleRow(currentRow, marginString);
			assembleRow(currentRow, "], eax");
			free(marginString);
			gen->codeList->add(gen->codeList, currentRow);
		}
	}
	return;
}



void writeLine(FILE* fp, const char* row) {
	fputc('\t', fp);
	fwrite(row, sizeof(char), LENGTH(row), fp);
	fputc('\n', fp);
	return;
}

void assembleRow(char* asmRow, char* newRow) {
	asmRow = (char*)realloc(asmRow, sizeof(char) * strlen(asmRow) + LENGTH(newRow));
	strcat(asmRow, newRow);
	return;
}





