#ifndef SEMANTIC_HEAD
#define SEMANTIC_HEAD

#include "ast.h"
#include "hash.h"

extern int SemanticErrors;

void check_and_set_declarations(AST *node);
void check_undeclared();
int getSemanticErrors();
int isIdentifierAlreadyDeclared(AST *node);
void updateDatatype(AST *node);

int isChar(int datatype);
int isInt(int datatype);
int isFloat(int datatype);
int isScalar(int datatype);
int isVector(int datatype);
int isFunction(int datatype);
//AST *findFunctionDeclaration(char * name, AST * node);
//int isSameDatatype(int datatype1, int datatype2);
//int isCompatible(int var, int atrib);

#endif
