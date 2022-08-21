#include "semantic.h"
#include "hash.h"

int SemanticErrors = 0;

void check_and_set_declarations(AST *node){
    int i;

    if (node == 0)
        return;
        
}
void check_undeclared(){
    SemanticErrors += hash_check_undeclared()
}

int getSemanticErrors(){
    return SemanticErrors;
}