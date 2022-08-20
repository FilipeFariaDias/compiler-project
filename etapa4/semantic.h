#ifndef SEMANTIC_HEAD
#define SEMANTIC_HEAD

#include "ast.h"
#include "hash.h"

extern int SemanticErrors;

void check_and_set_declarations(AST *node);
void check_undeclared();

#endif