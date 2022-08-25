#ifndef SEMANTIC_HEAD
#define SEMANTIC_HEAD

#include "ast.h"
#include "hash.h"

extern int SemanticErrors;

void check_and_set_declarations(AST *node);
void check_undeclared();
void check_usage(AST *node);
int getSemanticErrors();
int isIdentifierAlreadyDeclared(AST *node);
void updateDatatype(AST *node);

int isChar(int datatype);
int isInt(int datatype);
int isFloat(int datatype);
int isBool(int datatype);
int isVector(int datatype);
int isFunction(int datatype);
int checkVecElements(AST * node, int datatype);
int checkVecInit(AST *node);
int isCompatible(int datatype1, int datatype2);


#endif
