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
            fprintf(stderr, "Semantic Error: Identifier %s is already declared", node->symbol->text);
            ++SemanticErrors;
            return 1;
        }
    }
    return 0;
}

void updateDatatype(AST *node){
    if(node->sons[0]){
        switch(node->sons[0]->type){
            case AST_TPINT:     node->symbol->datatype = DATATYPE_INT;      break;
            case AST_TPFLOAT:   node->symbol->datatype = DATATYPE_FLOAT;    break;
            case AST_TPCHAR:    node->symbol->datatype = DATATYPE_CHAR;     break;
        }
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

int isCompatible(int datatype1, int datatype2){
    return (isChar(datatype1) && isInt(datatype2)) || (datatype1 == datatype2);
}

void check_undeclared(){
    SemanticErrors += hash_check_undeclared();
}

int getSemanticErrors(){
    return SemanticErrors;
}
