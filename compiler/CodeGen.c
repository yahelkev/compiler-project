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
	gen->codeList->free(gen->codeList);
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
	fprintf(fp, "\t.file \"%s\"", path);
	fprintf(fp, "\n\t.text\n\t.def\t___main;\t.scl  2;\t.type  32;\t.endef\n\t.section .rdata,\"dr\"");
	return fp;
}

void emitAsm(CodeGen* gen) {
	
	// Print LC Constants
	for (size_t i = 0; i < gen->lcList->size; i++)
		printLC(gen->lcList->consts[i], i, gen->filePointer);
	
	// Print saved functions after constants
	

	fwrite("\nmain:\n", sizeof(char), LENGTH("\nmain:\n"), gen->filePointer);
	writeLine(gen->filePointer, "PUSH rbp");
	writeLine(gen->filePointer, "MOV rbp, rsp\n");
	

	// Print entire code
	for (size_t i = 0; i < gen->codeList->amount; i++)
		writeLine(gen->filePointer, gen->codeList->strings[i]);

	fputc('\n', gen->filePointer);
	writeLine(gen->filePointer, "MOV eax, 0");
	writeLine(gen->filePointer, "POP rbp");
	writeLine(gen->filePointer, "RET");
	return;
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

char* GetOPRow(CodeGen* gen, ParseTree* child, char* currentRow) {
	switch (child->type) {
		case PARSE_PLUS: {
			currentRow = assembleRow(currentRow, "ADD eax, ");
			break;
		}
		case PARSE_MINUS: {
			currentRow = assembleRow(currentRow, "SUB eax, ");
			break;
		}
		case PARSE_STAR: {
			currentRow = assembleRow(currentRow, "IMUL eax, ");
			break;
		}
		case PARSE_SLASH: {
			gen->codeList->add(gen->codeList, "CDQ");
			currentRow = assembleRow(currentRow, "IDIV ");
			break;
		}
	}
	return currentRow;
}
void PostToAsmExp(CodeGen* gen, Heap_List* heapList, ParseTree* current) {
	ExpressionFirst(gen, heapList, current->getChild(current, 0));
	ParseTree* stack[MAX_STACK_SIZE], * child = NULL;
	int top = EMPTY_STACK;
	char* currentRow = NULL;
	size_t i = 1;
	for (child = current->getChild(current, i); i < current->amountOfChilds; i++, child = current->getChild(current, i)) {
		switch (child->type) {
		case IDENTIFIER_PARSE:
		case ATOMIC_PARSE:
			stack[++top] = child;
			break;
		default:
			currentRow = NULL;
			currentRow = GetOPRow(gen, child, currentRow);
			switch (stack[top]->type) {
			case IDENTIFIER_PARSE: {
				char marginString[15] = "";
				currentRow = assembleRow(currentRow, "DWORD PTR [rbp-");
				sprintf(marginString, "%d", getHeap(heapList, child->token->lexeme)->margin);
				currentRow = assembleRow(currentRow, marginString);
				currentRow = assembleRow(currentRow, "]");
				break;
			}
			case ATOMIC_PARSE: {
				char numSTR[15] = "";
				switch (stack[top]->token->type) {
				case TOKEN_STRING:
					currentRow = assembleRow(currentRow, "OFFSET FLAT:.LC");
					sprintf(numSTR, "%d", getLCOffset(gen->lcList, stack[top]->token->lexeme));
					currentRow = assembleRow(currentRow, numSTR);
					LC_ListAdd(gen->lcList, newLC(LC_String, child->token->lexeme, child->token));
					break;
				case TOKEN_FLOAT:
					currentRow = assembleRow(currentRow, "DWORD PTR .LC");
					sprintf(numSTR, "%d", getLCOffset(gen->lcList, stack[top]->token->lexeme));
					currentRow = assembleRow(currentRow, numSTR);
					currentRow = assembleRow(currentRow, "[rip]");
					LC_ListAdd(gen->lcList, newLC(LC_Long, child->token->lexeme, child->token));
					break;
				case TOKEN_INT:
					currentRow = assembleRow(currentRow, stack[top]->token->lexeme);
					break;
				default:
					break;
				}
				break;
			}
			}
			gen->codeList->add(gen->codeList, currentRow);
		}
		
	}
	
	return;
}

void ExpressionFirst(CodeGen* gen, Heap_List* heapList, ParseTree* child) {
	switch (child->type) {
	case IDENTIFIER_PARSE: {
		char* currentRow = NULL;
		char marginString[15] = "";
		currentRow = assembleRow(currentRow, "MOV eax, DWORD PTR [rbp-");
		sprintf(marginString, "%d", getHeap(heapList, child->token->lexeme)->margin);
		currentRow = assembleRow(currentRow, marginString);
		currentRow = assembleRow(currentRow, "]");
		gen->codeList->add(gen->codeList, currentRow);
		break;
	}
	case ATOMIC_PARSE: {
		char* currentRow = NULL;
		char numSTR[15] = "";
		switch (child->token->type) {
		case TOKEN_STRING:
			currentRow = assembleRow(currentRow, "MOV eax, OFFSET FLAT:.LC");
			sprintf(numSTR, "%d", getLCOffset(gen->lcList, child->token->lexeme));
			currentRow = assembleRow(currentRow, numSTR);
			LC_ListAdd(gen->lcList, newLC(LC_String, child->token->lexeme, child->token));
			break;
		case TOKEN_FLOAT:
			currentRow = assembleRow(currentRow, "MOV eax, DWORD PTR .LC");
			sprintf(numSTR, "%d", getLCOffset(gen->lcList, child->token->lexeme));
			currentRow = assembleRow(currentRow, numSTR);
			currentRow = assembleRow(currentRow, "[rip]");
			LC_ListAdd(gen->lcList, newLC(LC_Long, child->token->lexeme, child->token));
			break;
		case TOKEN_INT:
			currentRow = assembleRow(currentRow, "MOV eax, ");
			currentRow = assembleRow(currentRow, child->token->lexeme);
			break;
		default:
			break;
		}
		gen->codeList->add(gen->codeList, currentRow);
		break;
	}
	}
}

void CaseExpression(CodeGen* gen, Heap_List* heapList, ParseTree* current) {
	size_t i = 0;
	ParseTree* child = current->getChild(current, i);
	PostToAsmExp(gen, heapList, current);
	return;
}



void CaseVariable(CodeGen* gen, Heap_List* heapList , ParseTree* current) {

	switch (current->getChild(current, 0)->type) {
		case PARSE_INT_V: 
		case PARSE_FLOAT_V: {
			// Parsing the value of the variable
			CaseExpression(gen, heapList, current->getChild(current, 3));

			// making the actual row of creating the varialbe
			char* currentRow = NULL;
			int newMargin = heapList->size > 0 ? heapList->heaps[heapList->size - 1]->margin + 4 : 4;
			Heap_ListAdd(heapList, newHeap(HEAP_DWORD, current->getChild(current, 1)->token->lexeme, newMargin));
			currentRow = assembleRow(currentRow, "MOV DWORD PTR [rbp-");
			char* marginString= (char*)malloc((int)((ceil(log10((int)newMargin)) + 1) * sizeof(char)));
			sprintf(marginString, "%d", newMargin);
			currentRow = assembleRow(currentRow, marginString);
			currentRow = assembleRow(currentRow, "], eax");
			free(marginString);
			gen->codeList->add(gen->codeList, currentRow);
			break;
		}
		case PARSE_STRING_V: {
			// Parsing the value of the variable
			CaseExpression(gen, heapList, current->getChild(current, 3));

			// making the actual row of creating the varialbe
			char* currentRow = NULL;
			int newMargin = heapList->size > 0 ? heapList->heaps[heapList->size - 1]->margin % 8 == 0 ? heapList->heaps[heapList->size - 1]->margin + 8 : heapList->heaps[heapList->size - 1]->margin + 12 : 8;
			Heap_ListAdd(heapList, newHeap(HEAP_QWORD, current->getChild(current, 1)->token->lexeme, newMargin));
			currentRow =  assembleRow(currentRow, "MOV QWORD PTR [rbp-");
			char* marginString = (char*)malloc((int)((ceil(log10((int)newMargin)) + 1) * sizeof(char)));
			sprintf(marginString, "%d", newMargin);
			currentRow = assembleRow(currentRow, marginString);
			currentRow = assembleRow(currentRow, "], eax");
			free(marginString);
			gen->codeList->add(gen->codeList, currentRow);
			break;
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

char* assembleRow(char* asmRow, char* newRow) {
	bool mallocFlag = asmRow ? true : false;
	asmRow = mallocFlag ? (char*)realloc(asmRow, sizeof(char) * strlen(asmRow) + LENGTH(newRow)) : (char*)malloc(sizeof(char) * LENGTH(newRow));
	mallocFlag ? strcat(asmRow, newRow) : strcpy(asmRow, newRow);
	return asmRow;
}





