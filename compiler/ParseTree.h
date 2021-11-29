#ifndef PARSE_TREE
#define PARSE_TREE
#include "Lexer.h"
#include <stdlib.h>
#include <string.h>

typedef enum {
	FUNCTION,
	VARIABLE,
	EXPRESSION, // expression
	ATOMIC,
	IDENTIFIER,
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

	void (*addChild)(ParseTree* tree, ParseTree* child);
	ParseTree* (*getChild)(ParseTree* tree, int index);
	void (*freeParseTree)(ParseTree* tree);

}ParseTree;

ParseTree* newTree(ParseTreeType type, Token* toke);
void __ADDCHILD__(ParseTree* tree, ParseTree* child);
ParseTree* __GETCHILD__(ParseTree* tree, int index);
// Free tree's memory recursively
void __FREEPARSETREE__(ParseTree* tree);
#endif // !PARSE_TREE

