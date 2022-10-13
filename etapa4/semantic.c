#include "semantic.h"
#include "hash.h"
#include "ast.h"
#include <string.h>

int SemanticErrors = 0;
AST *ROOT;

void checkSemanticAnalysis(AST *node){
    ROOT = node;
    check_and_set_declarations(node);
    check_undeclared();
    check_usage(node);
}

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
                node->symbol->params = countParams(node->sons[1]);
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
                (node->sons[1]->symbol->type != SYMBOL_LIT_INTEGER || node->sons[1]->symbol->type != SYMBOL_LIT_FLOAT ||
                 node->sons[1]->symbol->type != SYMBOL_LIT_CHAR)){
                    fprintf(stderr, "Semantic Error: Identifier %s should be a integer, float or a character!\n", node->symbol->text);
                    ++SemanticErrors;
                 }
            break;
        case AST_DECVEC:
            if(!(isChar(node->sons[1]->symbol->datatype) || isInt(node->sons[1]->symbol->datatype))){
                fprintf(stderr, "Semantic Error: invalid values type in vector index initialization %s\n", node->symbol->text);
                ++SemanticErrors;
            }
            if(!checkVecElements(node->sons[2], node->symbol->datatype)){
                fprintf(stderr, "Semantic Error: invalid values type in vector elements initialization %s\n", node->symbol->text);
                ++SemanticErrors;
            }
            if(!checkVecInit(node)){
                fprintf(stderr, "Semantic Error: initialization number is not the same number of array elements %s\n", node->symbol->text);
                ++SemanticErrors;
            }
            break;
        case AST_DECFUNC:
            if(node->sons[0] == NULL){
                fprintf(stderr, "Semantic Error: Wrong function %s declaration\n", node->symbol->text);
                ++SemanticErrors;
            }
            if(node->sons[2] == NULL){
                fprintf(stderr, "Semantic Error: Wrong function %s declaration\n", node->symbol->text);
                ++SemanticErrors;
            }
            checkFuncReturn(node, node->sons[0]->type, node->sons[2]->sons[0]);
            break;
        case AST_ATTR:
            if(node->symbol->type != SYMBOL_VARIABLE){
                fprintf(stderr, "Semantic Error: Attribuition has wrong identifier, should be a number %s\n", node->symbol->text);
                ++SemanticErrors;
            }
            if(!(isChar(node->symbol->datatype) || isFloat(node->symbol->datatype) || isInt(node->symbol->datatype)) &&
                !(node->sons[0]->symbol->type != SYMBOL_LIT_INTEGER || node->sons[0]->symbol->type != SYMBOL_LIT_FLOAT ||
                 node->sons[0]->symbol->type != SYMBOL_LIT_CHAR)){
                    fprintf(stderr, "Semantic Error: Attribuition has incompatible types %s\n", node->symbol->text);
                    ++SemanticErrors;
                 }
            else if(!(isBool(node->symbol->datatype) && node->sons[0]->symbol->type == SYMBOL_LIT_BOOL)){
                fprintf(stderr, "Semantic Error: Attribuition has incompatible types %s\n", node->symbol->text);
                ++SemanticErrors;
            }
            break;
        case AST_VECATTR:
            if(node->symbol->type != SYMBOL_VECTOR){
                fprintf(stderr, "Semantic Error: Attribuition has wrong identifier, should be a vector %s\n", node->symbol->text);
                ++SemanticErrors;
            }
            if((node->sons[0]->symbol->type == SYMBOL_LIT_BOOL && isBool(node->sons[0]->symbol->type)) ||
                (node->sons[0]->symbol->type == SYMBOL_LIT_FLOAT && isFloat(node->sons[0]->symbol->type))){
                    fprintf(stderr, "Semantic Error: Vector index should not be a boolean or a float %s\n", node->symbol->text);
                     ++SemanticErrors;
                }
            if(node->sons[1] != NULL){
                if((node->sons[1]->symbol->type == SYMBOL_LIT_BOOL && isBool(node->sons[1]->symbol->type)) ||
                    (node->sons[1]->symbol->type == SYMBOL_LIT_FLOAT && isFloat(node->sons[1]->symbol->type))){
                        fprintf(stderr, "Semantic Error: Vector attribution should not be a boolean or a float %s\n", node->symbol->text);
                         ++SemanticErrors;
                    }
            }
            break;
        case AST_PRINT:
            if(node->sons[0] != NULL && node->sons[0]->symbol->type != SYMBOL_LIT_STRING){
                fprintf(stderr, "Semantic Error: Print should only have string type %s\n", node->symbol->text);
                ++SemanticErrors;
            }
            break;
        case AST_IF:
        case AST_IFELSE:
        case AST_WHILE:
            if(node->sons[0] != NULL && !(isBool(node->sons[0]->symbol->datatype))){
                fprintf(stderr, "Semantic Error: Condition should be boolean\n");
                ++SemanticErrors;
            }
            break;
        case AST_VEC:
            if(node->symbol->type != SYMBOL_VECTOR){
                fprintf(stderr, "Semantic Error: Identifier %s should be a vector\n", node->symbol->text);
                ++SemanticErrors;
            }
            if(!(isChar(node->sons[0]->symbol->datatype) || isInt(node->sons[0]->symbol->datatype) || 
                (node->sons[0]->symbol->type != SYMBOL_LIT_INTEGER ||
                 node->sons[0]->symbol->type != SYMBOL_LIT_CHAR))){
                    fprintf(stderr, "Semantic Error: Invalid vector index\n");
                    ++SemanticErrors;
                 }
            break;
        case AST_FUNC:
            if(node->symbol->type != SYMBOL_FUNCTION){
                fprintf(stderr, "Semantic Error: Identifier %s should be a function\n", node->symbol->text);
                ++SemanticErrors;
            }
            else if(node->symbol->params != countParams(node->sons[1])){
                fprintf(stderr, "Semantic Error: Wrong number of parameters\n");
                ++SemanticErrors;
            }
            AST *declaration;
            if(node->symbol){
                declaration = findFunctionDeclaration(node->symbol->text, ROOT);
            }
            checkFunction(node->sons[0], declaration->sons[0]);
            break;
        case AST_SUM:
        case AST_DEC:
        case AST_DOT:
        case AST_DIV:
        if(node->sons[0] != NULL && node->sons[1] != NULL){
            if(!(isIntorFloat(node->sons[0])) || !(isIntorFloat(node->sons[1]))){
                fprintf(stderr, "Semantic Error: Operands should be a number!\n");
                ++SemanticErrors;
            }
        }
            break;
        case AST_LESS:
        case AST_GREAT:
        case AST_EQ:
        case AST_GE:
        case AST_LE:
        case AST_DIF:
        if(node->sons[0] != NULL && node->sons[1] != NULL){
            if(!isIntCharorFloat(node->sons[0]) || !isIntCharorFloat(node->sons[1])){
                fprintf(stderr, "Semantic Error: Operands should be a integer, float or char!\n");
                ++SemanticErrors;
            }
        }
            break;
        case AST_AND:
        case AST_OR:
        if(node->sons[0] != NULL && node->sons[1] != NULL){
            if(!(isBool(node->sons[0]->symbol->datatype) || node->sons[0]->symbol->type == SYMBOL_LIT_BOOL) || 
                !(isBool(node->sons[1]->symbol->datatype) || node->sons[1]->symbol->type == SYMBOL_LIT_BOOL)){
                fprintf(stderr, "Semantic Error: Operands should be boolean!\n");
                ++SemanticErrors;
            }
        }
            break;
        case AST_NOT:
        if(node->sons[0] != NULL){
            if(!(isBool(node->sons[0]->symbol->datatype) || node->sons[0]->symbol->type == SYMBOL_LIT_BOOL)){
                fprintf(stderr, "Semantic Error: Operand should be boolean!\n");
                ++SemanticErrors;
            }
        }
            break;
        case AST_PAREN:
            return check_usage(node->sons[0]);
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

int isBool(int datatype){
    return (datatype == DATATYPE_BOOL);
}

int isIntorFloat(AST *node){
    if(node->symbol->type == SYMBOL_LIT_INTEGER ||
        node->symbol->type == SYMBOL_LIT_FLOAT ||
        node->symbol->datatype == DATATYPE_INT ||
        node->symbol->datatype == DATATYPE_FLOAT)
        return 1;
    return 0;
}

int isIntCharorFloat(AST *node){
    if(node->symbol->type == SYMBOL_LIT_INTEGER ||
        node->symbol->type == SYMBOL_LIT_FLOAT ||
        node->symbol->type == SYMBOL_LIT_CHAR ||
        node->symbol->datatype == DATATYPE_INT ||
        node->symbol->datatype == DATATYPE_FLOAT ||
        node->symbol->datatype == DATATYPE_CHAR)
        return 1;
    return 0;
}

int checkVecElements(AST * node, int datatype){	
	if(node != NULL){
		if(!isCompatible(node->sons[0]->symbol->datatype, datatype))
			return 0;
		if(node->sons[1] != NULL)
			return checkVecElements(node->sons[1], datatype);
	}
	return 1;
}

int checkVecInit(AST *node){
    int length = atoi(node->sons[1]->symbol->text);
    int elem_length = 0;
    AST *elem_values = node->sons[2];

    while(elem_values){
        elem_length++;
        elem_values = elem_values->sons[1];
    }
    if(elem_length != length)
        return 0;

    return 1;
}

int countParams(AST *node){
    if(node == NULL)
        return 0;
    else 
        return 1 + countParams(node->sons[1]);
}


AST* findFunctionDeclaration(char * name, AST * node){
	if(node->symbol != NULL && node->type == AST_DECFUNC && strcmp(node->symbol->text, name) == 0)
		return node;

	for(int i = 0; i < MAX_SONS; i++){
		if(node->sons[i] == NULL)
			return NULL;
		AST * finding = findFunctionDeclaration(name, node->sons[i]) ;
		if(finding != NULL)
			return finding;
	}
	return NULL;
}

void checkFunction(AST* funCall, AST *funDecNode){
    if(funCall == NULL || funDecNode == NULL)
        return;
     if(funCall->sons[0] != NULL && funDecNode->sons[0] != NULL){
        if(isBool(funDecNode->sons[0]->symbol->datatype) && funCall->sons[0]->symbol->type != SYMBOL_LIT_BOOL){
            fprintf(stderr, "Semantic Error: Parameters should be boolean!\n");
            ++SemanticErrors;
        }
        else if(isInt(funDecNode->sons[0]->symbol->datatype) || 
                isFloat(funDecNode->sons[0]->symbol->datatype) ||
                isChar(funDecNode->sons[0]->symbol->datatype) && !isIntCharorFloat(funDecNode->sons[0])){
                    fprintf(stderr, "Semantic Error: Parameters should be a number!\n");
                    ++SemanticErrors;
            }
        checkFunction(funCall->sons[1], funDecNode->sons[1]);
     }
}

void checkFuncReturn(AST *node, int funcType, AST *tail){
    AST* head;
    if(tail == NULL)
        return;

    head = tail->sons[0];

    if(head->type == AST_RETURN && head != NULL){
        if(isInt(funcType) || 
            isFloat(funcType) ||
            isChar(funcType) && !isIntCharorFloat(head->sons[0])){
                fprintf(stderr, "Semantic Error: identifier should be a number!\n");
                ++SemanticErrors;
        }
    }
    checkFuncReturn(node, funcType, tail->sons[1]);
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
