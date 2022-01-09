#include "CodeGen.h"

void newCodeGen(CodeGen* gen, char* path, ParseTree* mainTree, Table* table) {
	gen->filePath = (char*)malloc(sizeof(char) * LENGTH(path));
	strncpy(gen->filePath, path, LENGTH(path));
	gen->_main = mainTree;
	gen->table = table;
	gen->codeList = newStringList();
	gen->lcList = newLC_List();
	gen->loopCounter = 0;
	gen->conditionCounter = 0;
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
	fprintf(fp, "\n\t.intel_syntax noprefix\n\t.text\n\t.def\t___main;\t.scl  2;\t.type  32;\t.endef\n\t.section .rdata,\"dr\"");
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

void Generate(CodeGen* gen, ParseTree* current) {
	size_t index = 0;
	ParseTree* currentChild = current->getChild(gen->_main, index);
	Heap_List* heapList = newHeap_List();
	for (index = 0; index < current->amountOfChilds; index++, currentChild = current->getChild(current, index)) {

		switch (currentChild->type) {
		case VARIABLE_PARSE:
			CaseVariable(gen, heapList, currentChild);
			break;
		case ASSIGN_PARSE:
			CaseAssign(gen, heapList, currentChild);
			break;
		case FULL_LOOP_PARSE:
			CaseLoop(gen, heapList, currentChild);
			break;
		case FULL_CONDITIONAL_PARSE:
			CaseConditions(gen, heapList, currentChild);
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
			currentRow = GetOPRow(gen, child, currentRow);
			switch (stack[top]->type) {
			case IDENTIFIER_PARSE: {
				char marginString[MAX_DIGIT_LENGTH] = "";
				currentRow = assembleRow(currentRow, "DWORD PTR [rbp-");
				sprintf(marginString, "%d", getHeap(heapList, child->token->lexeme)->margin);
				currentRow = assembleRow(currentRow, marginString);
				currentRow = assembleRow(currentRow, "]");
				break;
			}
			case ATOMIC_PARSE: {
				char numSTR[MAX_DIGIT_LENGTH] = "";
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
		char* currentRow = NULL, marginString[MAX_DIGIT_LENGTH] = "";
		currentRow = assembleRow(currentRow, "MOV eax, DWORD PTR [rbp-");
		sprintf(marginString, "%d", getHeap(heapList, child->token->lexeme)->margin);
		currentRow = assembleRow(currentRow, marginString);
		currentRow = assembleRow(currentRow, "]");
		gen->codeList->add(gen->codeList, currentRow);
		break;
	}
	case ATOMIC_PARSE: {
		char* currentRow = NULL;
		char numSTR[MAX_DIGIT_LENGTH] = "";
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

	PostToAsmExp(gen, heapList, current);
	return;
}

char* getJmpCondition(ParseTreeType type)
{
	switch (type)
	{
	case PARSE_GREATER:
		return "jg";
	case PARSE_GREATER_EQUAL:
		return "jge";
	case PARSE_LESS:
		return "jl";
	case PARSE_LESS_EQUAL:
		return "jle";
	case PARSE_EQUAL_EQUAL:
		return "je";
	case PARSE_BANG_EQUAL:
		return "jne";
	}
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

void CaseAssign(CodeGen* gen, Heap_List* heapList, ParseTree* current) {
	/*
	* int
	    mov     DWORD PTR [rbp-4], 0
	* string
	    mov     QWORD PTR [rbp-16], OFFSET FLAT:.LC0
    * float    
		movss   xmm0, DWORD PTR .LC1[rip]
        movss   DWORD PTR [rbp-20], xmm0
	*/
	char* currentRow = NULL;
	char numSTR[MAX_DIGIT_LENGTH] = "";
	CaseExpression(gen, heapList, current->getChild(current, 2));
	
	ParseTree* firstChild = current->getChild(current, 0);
	if (!strcmp(getValue(gen->table, (firstChild->token->lexeme)).variable->type, "string")){
		currentRow = assembleRow(currentRow, "MOV	QWORD PTR [rbp-");
	}
	else if(!strcmp(getValue(gen->table, (firstChild->token->lexeme)).variable->type, "float")){
		currentRow = assembleRow(currentRow, "MOVSS	DWORD PTR [rbp-");
	}
	else if (!strcmp(getValue(gen->table, (firstChild->token->lexeme)).variable->type, "int")){
		currentRow = assembleRow(currentRow, "MOV	DWORD PTR [rbp-");
	}
	sprintf(numSTR, "%d", getHeap(heapList, firstChild->token->lexeme)->margin);
	currentRow = assembleRow(currentRow, numSTR);
	currentRow = assembleRow(currentRow, "], eax");
	gen->codeList->add(gen->codeList, currentRow);
}

void CaseLoop(CodeGen* gen, Heap_List* heapList, ParseTree* current) {
	/*
		jmp end_loop
		start_loop:
			*body*
		end_loop:
		expresion/condition
		cmp edx,eax
		jmp condotion start_loop

	*/
	char numSTR[15] = "";
	char* currentRow = NULL;
	sprintf(numSTR, "%d", gen->loopCounter);

	ParseTree* condition = current->getChild(current, 0);
	ParseTree* expression = condition->getChild(condition, 0);
	ParseTree* body = current->getChild(current, 1);

	currentRow = assembleRow(currentRow, "jmp end_loop");
	currentRow = assembleRow(currentRow, numSTR);
	gen->codeList->add(gen->codeList, currentRow);
	currentRow = NULL;

	currentRow = assembleRow(currentRow, "start_loop");
	currentRow = assembleRow(currentRow, numSTR);
	currentRow = assembleRow(currentRow, ":");
	gen->codeList->add(gen->codeList, currentRow);
	currentRow = NULL;
	
	Generate(gen, body);

	currentRow = assembleRow(currentRow, "end_loop");
	currentRow = assembleRow(currentRow, numSTR);
	currentRow = assembleRow(currentRow, ":");
	gen->codeList->add(gen->codeList, currentRow);
	currentRow = NULL;

	CaseExpression(gen, heapList, expression);
	currentRow = assembleRow(currentRow, "cmp edx, eax");
	gen->codeList->add(gen->codeList, currentRow);
	currentRow = NULL;
	
	currentRow = assembleRow(currentRow, getJmpCondition(expression->getChild(expression, expression->amountOfChilds - 1)->type));
	currentRow = assembleRow(currentRow, " start_loop");
	currentRow = assembleRow(currentRow, numSTR);
	gen->codeList->add(gen->codeList, currentRow);

	gen->loopCounter++;
}

void CaseConditions(CodeGen* gen, Heap_List* heapList, ParseTree* current) {
	/*
		cmp     edx, eax
		jmp_condition if_body
		*else Body
		jmp end_if
		if_body:
			*if_body
		end_if:

	*/

	char numSTR[MAX_DIGIT_LENGTH] = "";
	char* currentRow = NULL;
	sprintf(numSTR, "%d", gen->conditionCounter);

	ParseTree* condition = current->getChild(current, 0);
	ParseTree* expression = condition->getChild(condition, 0);
	ParseTree* body = current->getChild(current, 1);


	CaseExpression(gen, heapList, expression);
	currentRow = assembleRow(currentRow, "cmp eax, 0");
	gen->codeList->add(gen->codeList, currentRow);
	currentRow = NULL;

	currentRow = assembleRow(currentRow, getJmpCondition(expression->getChild(expression, expression->amountOfChilds - 1)->type));
	currentRow = assembleRow(currentRow, " if_body");
	currentRow = assembleRow(currentRow, numSTR);
	gen->codeList->add(gen->codeList, currentRow);
	currentRow = NULL;

	// Converting Else Block
	if (current->amountOfChilds == 3) Generate(gen, current->getChild(current, 2));

	currentRow = assembleRow(currentRow, "jmp end_if");
	currentRow = assembleRow(currentRow, numSTR);
	gen->codeList->add(gen->codeList, currentRow);
	currentRow = NULL;

	currentRow = assembleRow(currentRow, "if_body");
	currentRow = assembleRow(currentRow, numSTR);
	currentRow = assembleRow(currentRow, ":");
	gen->codeList->add(gen->codeList, currentRow);
	currentRow = NULL;

	Generate(gen, body);

	currentRow = assembleRow(currentRow, "end_if");
	currentRow = assembleRow(currentRow, numSTR);
	currentRow = assembleRow(currentRow, ":");
	gen->codeList->add(gen->codeList, currentRow);

	gen->conditionCounter++;
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





