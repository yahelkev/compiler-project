#include "Visitor.h"

Error_Codes newVisitor(Visitor* vis, Parser* par) {
	vis = (Visitor*)malloc(sizeof(Visitor));
	if (vis == NULL) return VIS_MALLOC_FAIL;
	if (par->error) return VIS_PARSER_ERROR;
	
	vis->par = par;
	return VIS_OK;
}

void visitAll(Visitor* vis) {
	while (getParseTree(vis->par)) {
		// Check current ast
		// Returns most recent parseTree structure
		visitAst(vis->par->mainTree->getChild(vis->par->mainTree, vis->par->mainTree->amountOfChilds - 1));
	}
}

bool visitAst(Visitor* vis) {
	// Works close to how the statement funciton worked in the parser, as checking current tree and going down in lelvels if necessary
}

bool getParseTree(Parser* par) {
	synchronize(par);
	if (par->current->type != TOKEN_EOF) {
		if (par->current->type != TOKEN_END_LINE) scanParser(par, par->mainTree);
		else parserAdvance(par);
		return true;
	}
	return false;
}
