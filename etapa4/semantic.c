#include "semantic.h"
#include "hash.h"
#include "ast.h"

int SemanticErrors = 0;

void check_and_set_declarations(AST *node){
    int i;

    if (node == 0)
        return;

    switch(node->type){
        case AST_DECVAR:
            if(!isIdentifierAlreadyDeclared(node)){
                node->symbol->type = SYMBOL_VARIABLE;
                updateDatatype(node);
            }
            break;
        case AST_DECVEC:
            if(!isIdentifierAlreadyDeclared(node)){
                node->symbol->type = SYMBOL_VECTOR;
                updateDatatype(node);
            }
            break;
        case AST_DECFUNC:
            if(!isIdentifierAlreadyDeclared(node)){
                node->symbol->type = SYMBOL_FUNCTION;
                updateDatatype(node);
            }
            break;
        case AST_PARAM:
            if(!isIdentifierAlreadyDeclared(node)){
                node->symbol->type = SYMBOL_VARIABLE;
                updateDatatype(node);
            }
            break;
        default:
            break;
    }
    
    for(i = 0; i < MAX_SONS; i++){
        check_and_set_declarations(node->sons[i]);
    }
}

int isIdentifierAlreadyDeclared(AST *node){
    if(node->symbol){
        if(node->symbol->type != SYMBOL_IDENTIFIER){
            fprintf(stderr, "Semantic Error: Identifier %s is already declared\n", node->symbol->text);
            ++SemanticErrors;
            return 1;
        }
    }
    return 0;
}

void updateDatatype(AST *node){
    if(node->sons[0]){
        if(node->sons[0]->type == AST_TPINT) node->symbol->datatype = DATATYPE_INT;
        if(node->sons[0]->type == AST_TPFLOAT) node->symbol->datatype = DATATYPE_FLOAT;
        if(node->sons[0]->type == AST_TPCHAR) node->symbol->datatype = DATATYPE_CHAR;
    }
}

void check_usage(AST *node){
    int i;

    if(node ==  0)
        return;

    switch(node->type){
        case AST_DECVAR: 
            if(node->symbol->type != SYMBOL_VARIABLE){
                fprintf(stderr, "Semantic Error: Identifier %s should be numeric!\n", node->symbol->text);
                ++SemanticErrors;
            }
            if(isInt(node->symbol->datatype) || isFloat(node->symbol->datatype) || isChar(node->symbol->datatype) && 
                (node->sons[1]->symbol->type != SYMBOL_LIT_INTEGER && node->sons[1]->symbol->type != SYMBOL_LIT_FLOAT &&
                 node->sons[1]->symbol->type != SYMBOL_LIT_CHAR)){
                    fprintf(stderr, "Semantic Error: Identifier %s should be a integer, float or a character!\n", node->symbol->text);
                    ++SemanticErrors;
                 }
            break;
        default:
            break;
    }
}

int isChar(int datatype){
    return (datatype == DATATYPE_CHAR);
}
            
int isInt(int datatype){
    return (datatype == DATATYPE_INT);
}

int isFloat(int datatype){
    return (datatype == DATATYPE_FLOAT);
}

int checkVecElements(AST * node, int datatype){	
	if(node != NULL){
		if(!isDatatypeCompatible(node->sons[0]->symbol->datatype, datatype))
			return 0;
		if(node->sons[1] != NULL)
			return checkEveryVecElement(node->sons[1], datatype);
	}
	return 1;
}

int isCompatible(int datatype1, int datatype2){
    return (isChar(datatype1) && isInt(datatype2)) || (datatype1 == datatype2);
}

void check_undeclared(){
    SemanticErrors += hash_check_undeclared();
}

int getSemanticErrors(){
    return SemanticErrors;
}
