#include "ParseTree.h"

ParseTree* newTree(ParseTreeType type, Token* toke) {
	// Reset Tree
	ParseTree* tree = (ParseTree*)malloc(sizeof(ParseTree));
	tree->type = type;
	tree->token = toke;

	// Setup childs
	tree->childs = (ParseTree**)realloc(tree->childs, sizeof(ParseTree*));
	tree->amountOfChilds = 0;

	// Setup methods
	tree->addChild = &__ADDCHILD__;
	tree->freeParseTree = &__FREEPARSETREE__;
	tree->getChild = &__GETCHILD__;

	return tree;
}


void __ADDCHILD__(ParseTree* tree, ParseTree* child) {
	// Add another child spot
	tree->childs = (ParseTree**)realloc(tree->childs, sizeof(ParseTree*) * (tree->amountOfChilds + 1));
	tree->childs[tree->amountOfChilds] = (ParseTree*)malloc(sizeof(ParseTree));
	tree->childs[tree->amountOfChilds] = child;
	tree->amountOfChilds++;
}
ParseTree* __GETCHILD__(ParseTree* tree, int index) {
	return index >= tree->amountOfChilds ? NULL : tree->childs[index];
}
void __FREEPARSETREE__(ParseTree* tree) {
	if (!tree) return;
	if (tree->amountOfChilds) {
		for (size_t i = 0; i < tree->amountOfChilds; i++) {
			tree->childs[tree->amountOfChilds]->freeParseTree(tree->childs[tree->amountOfChilds]);
		}
	}
	
	freeToken(tree->token);
	free(tree);
}