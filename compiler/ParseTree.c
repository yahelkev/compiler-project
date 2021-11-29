#include "ParseTree.h"

ParseTree* newTree(ParseTreeType type, Token* toke) {
	ParseTree* tree = (ParseTree*)malloc(sizeof(ParseTree));
	tree->amountOfChilds = 0;
	tree->type = type;
	tree->token = toke;

	tree->addChild = &__ADDCHILD__;
	tree->freeParseTree = &__FREEPARSETREE__;
	tree->getChild = &__GETCHILD__;
}


void __ADDCHILD__(ParseTree* tree, ParseTree* child) {

}
ParseTree* __GETCHILD__(ParseTree* tree, int index) {

}
void __FREEPARSETREE__(ParseTree* tree) {

}