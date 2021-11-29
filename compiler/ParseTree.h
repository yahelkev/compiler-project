#ifndef PARSE_TREE
#define PARSE_TREE
#include "Lexer.h"
#include <stdlib.h>
#include <string.h>

typedef enum {
	FUNCTION_PARSE,
	VARIABLE_PARSE,
	EXPRESSION_PARSE, // expression
	ATOMIC_PARSE,
	IDENTIFIER_PARSE,
	MAIN_PARSE,
	INT_PARSE,
	ASSIGN_PARSE
} ParseTreeType;
//// int x
//// float y
//// char letter
//// string name
//Different types of arg_list :
//()
//(int a)
//(int a, int b)
//fn add(int a, int b)...
//function :
//	fn
//	identifier - add
//	arg_list
//	body
typedef struct ParseTree {
	ParseTreeType type; // expression, function, atomic
	Token* token;
	int amountOfChilds;
	struct ParseTree** childs;

	void (*addChild)(struct ParseTree* tree, struct ParseTree* child);
	struct ParseTree* (*getChild)(struct ParseTree* tree, int index);
	void (*freeParseTree)(struct ParseTree* tree);

}ParseTree;

ParseTree* newTree(ParseTreeType type, Token* toke);
void __ADDCHILD__(ParseTree* tree, ParseTree* child);
ParseTree* __GETCHILD__(ParseTree* tree, int index);
// Free tree's memory recursively
void __FREEPARSETREE__(ParseTree* tree);
#endif // !PARSE_TREE

