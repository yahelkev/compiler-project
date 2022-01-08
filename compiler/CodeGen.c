﻿#include "CodeGen.h"

void newCodeGen(CodeGen* gen, char* path, ParseTree* mainTree, Table* table) {
	gen->filePath = (char*)malloc(sizeof(char) * LENGTH(path));
	strncpy(gen->filePath, path, LENGTH(path));
	gen->_main = mainTree;
	gen->table = table;
	gen->codeList = newStringList();
	gen->lcList = newLC_List();
	gen->loopCounter = 0;
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

void Generate(CodeGen* gen, ParseTree* current) {
	size_t index = 0;
	ParseTree* currentChild = current->getChild(gen->_main, index);
	Heap_List* heapList = newHeap_List();
	for (index = 0; index <current->amountOfChilds; index++, currentChild = current->getChild(current, index)) {

		switch (currentChild->type) {
		case VARIABLE_PARSE:
			CaseVariable(gen, heapList, currentChild);
			break;
		case ASSIGN_PARSE:
			CaseAssign(gen, heapList, currentChild);
			break;
		case FULL_LOOP_PARSE:
			CaseLoop(gen, heapList, currentChild);
		}

	}
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

void CaseAssign(CodeGen* gen, Heap_List* heapList, ParseTree* current)
{
	/*
	* int
	    mov     DWORD PTR [rbp-4], 0
	* string
	    mov     QWORD PTR [rbp-16], OFFSET FLAT:.LC0
    * float    
		movss   xmm0, DWORD PTR .LC1[rip]
        movss   DWORD PTR [rbp-20], xmm0
	*/
	char* currentRow = (char*)malloc(1);
	*currentRow = '\0';
	char numSTR[15] = "";
	CaseExpression(gen, heapList, current->getChild(current, 2));
	
	ParseTree* firstChild = current->getChild(current, 0);
	if (!strcmp(getValue(gen->table, (firstChild->token->lexeme)).variable->type, "string")){
		assembleRow(currentRow, "mov     QWORD PTR [rbp-");
	}
	else if(!strcmp(getValue(gen->table, (firstChild->token->lexeme)).variable->type, "float")){
		assembleRow(currentRow, "movss   DWORD PTR [rbp-");
	}
	else if (!strcmp(getValue(gen->table, (firstChild->token->lexeme)).variable->type, "int")){
		assembleRow(currentRow, "mov     DWORD PTR [rbp-");
	}
	sprintf(numSTR, "%d", getHeap(heapList, firstChild->token->lexeme)->margin);
	assembleRow(currentRow, numSTR);
	assembleRow(currentRow, "], ax");
	gen->codeList->add(gen->codeList, currentRow);
}

void CaseLoop(CodeGen* gen, Heap_List* heapList, ParseTree* current)
{
	/*
		jmp end_loop
		start_loop:
			*body*
		end_loop:
		expresion/condition
		cmp dx,ax
		jmp condotiom start_loop

	*/
	char numSTR[15] = "";
	char* currentRow = (char*)malloc(1);
	*currentRow = '\0';
	sprintf(numSTR, "%d", gen->loopCounter);

	ParseTree* condition = current->getChild(current, 0);
	ParseTree* expression = condition->getChild(condition, 0);
	ParseTree* body = current->getChild(current, 1);
	assembleRow(currentRow, "jmp end_loop");
	assembleRow(currentRow, numSTR);
	gen->codeList->add(gen->codeList, currentRow);
	*currentRow = '\0';
	assembleRow(currentRow, "start_loop");
	assembleRow(currentRow, numSTR);
	assembleRow(currentRow, ":");
	gen->codeList->add(gen->codeList, currentRow);
	*currentRow = '\0';
	
	Generate(gen, body);

	assembleRow(currentRow, "end_loop");
	assembleRow(currentRow, numSTR);
	assembleRow(currentRow, ":");
	gen->codeList->add(gen->codeList, currentRow);
	*currentRow = '\0';

	CaseExpression(gen, heapList, expression);
	assembleRow(currentRow, "cmp edx, eax");
	gen->codeList->add(gen->codeList, currentRow);
	*currentRow = '\0';
	
	assembleRow(currentRow, getJmpCondition(expression->getChild(expression, expression->amountOfChilds - 1)->type));
	assembleRow(currentRow, " start_loop");
	assembleRow(currentRow, numSTR);
	gen->codeList->add(gen->codeList, currentRow);

	gen->loopCounter++;
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





