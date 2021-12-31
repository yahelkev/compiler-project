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
	expressionAsm(&gen, gen->_main->getChild(gen->_main, 0)->getChild(gen->_main->getChild(gen->_main, 0),3));
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

void emitByte(FILE* fp, const char* row) {
	fputc('\t', fp);
	fwrite(row, sizeof(char), LENGTH(row), fp);
	fputc('\n', fp);
	return;
}

void expressionAsm(CodeGen* gen, ParseTree* tree)
{
	ParseTree* child;
    int n1, n2, n3, num;

	for (int i = 0; i < tree->amountOfChilds; i++)
    {
		child = tree->getChild(tree, i);
        if (child->type == IDENTIFIER_PARSE){
			printf("PUSH	[%s]\n", child->token->lexeme);
        }
		else if(child->type == ATOMIC_PARSE){
			printf("PUSH	%s\n", child->token->lexeme);
		}
        else
        {
			printf("POP	edx\n");
			printf("POP	eax\n");
            switch (child->type)
            {
			case PARSE_PLUS:
            {
				printf("add	eax, edx\n");
                break;
            }
            case PARSE_MINUS:
            {
				printf("sub	eax, edx\n");
                break;
            }
            case PARSE_STAR:
            {
				printf("imul	eax, edx\n");
                break;
            }
            case PARSE_SLASH:
            {
				printf("cdq\n");
				printf("idiv	edx\n");
                break;
            }
            }
			printf("PUSH	eax\n");
        }
    }
	printf("POP	eax\n");
    return 0;
}


void CaseVariable(const FILE* fp, ParseTree* current) {
	return;
}





