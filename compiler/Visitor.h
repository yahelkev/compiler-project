#ifndef VISITOR_H
#define VISITOR_H
#include <stdbool.h>
#include "Parser.h"

// Return codes to signal wheter we should visit the tree or not, when creating a new visitor struct
typedef enum {
	VIS_OK,
	VIS_MALLOC_FAIL,
	VIS_PARSER_ERROR,

}Error_Codes;

// Visitor struct used to visit the trees returned from the Parser
typedef struct Visitor {
	Parser* par;
	bool error;
}Visitor;

// Got to check the code returned before starting to visit
Error_Codes newVisitor(Visitor* vis, Parser* par);
// Main class function to start visiting all trees in the code
void visitAll(Visitor* vis);
// Single visit function to visit a single tree, will work in multiple levels
bool visitAst(Visitor* vis);
// Returns the current parseTree from scanParser to work on
bool getParseTree(Parser* par);
#endif // !VISITOR_H


