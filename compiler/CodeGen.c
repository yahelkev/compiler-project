#include "CodeGen.h"

void newCodeGen(CodeGen* gen, char* path, ParseTree* mainTree, Table* table) {
	gen->filePath = (char*)malloc(sizeof(char) * LENGTH(path));
	strncpy(gen->filePath, path, LENGTH(path));
	gen->_main = mainTree;
	gen->table = table;
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
	CaseExpression(gen, gen->_main->getChild(gen->_main, 0)->getChild(gen->_main->getChild(gen->_main, 0),3));
	size_t index = 0;
	ParseTree* currentChild = gen->_main->getChild(gen->_main, index);
	for (index = 0; index < gen->_main->amountOfChilds; index++, currentChild = gen->_main->getChild(gen->_main, index)) {
		switch (currentChild->type) {
		case VARIABLE_PARSE:
			CaseVariable(gen->filePointer, currentChild);
			break;
		}
	}
}

void writeLine(FILE* fp, const char* row) {
	fputc('\t', fp);
	fwrite(row, sizeof(char), LENGTH(row), fp);
	fputc('\n', fp);
	return;
}

void emitByte(CodeGen* gen, Literal lit, const char* row) {
	switch (lit) {
	case String_C: gen->stringList->add(gen->stringList, row); return;
	case Code_C: gen->codeList->add(gen->codeList, row); return;
	}
	return;
}


























void CaseExpression(CodeGen* gen, ParseTree* tree) {
	size_t i = 0;
	ParseTree* child = tree->getChild(tree, i);
	for (; i < tree->amountOfChilds; i++, child = tree->getChild(tree, i)) {
        if (child->type == IDENTIFIER_PARSE){
			fputs("PUSH	[", gen->filePointer);
			fputs(child->token->lexeme, gen->filePointer);
			fputs("]\n", gen->filePointer);
			// x = 5
			// PUSH [x]
			// PUSH [5]
        }
		else if(child->type == ATOMIC_PARSE) {
			fputs("PUSH	", gen->filePointer);
			fputs(child->token->lexeme, gen->filePointer);
			fputs("\n", gen->filePointer);
		}
        else
        {
			fputs("POP	edx\n", gen->filePointer);
			fputs("POP	eax\n", gen->filePointer);
            switch (child->type)
            {
			case PARSE_PLUS:
            {
				fputs("add	eax, edx\n", gen->filePointer);
                break;
            }
            case PARSE_MINUS:
            {
				fputs("sub	eax, edx\n", gen->filePointer);
                break;
            }
            case PARSE_STAR:
            {
				fputs("imul	eax, edx\n", gen->filePointer);
                break;
            }
            case PARSE_SLASH:
            {
				fputs("cdq\n", gen->filePointer);
				fputs("idiv	edx\n", gen->filePointer);
                break;
            }
            }
			fputs("PUSH	eax\n", gen->filePointer);
        }
    }
	fputs("POP	eax\n", gen->filePointer);
}


void CaseVariable(const FILE* fp, ParseTree* current) {

	

	switch (current->getChild(current, 0)->type) {
	case PARSE_INT_V: {

	}
	}
	return;
}





