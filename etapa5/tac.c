#include <stdio.h>
#include <stdlib.h>
#include "tac.h"


TAC* tacCreate(int type, HASH_NODE *res, HASH_NODE *op1, HASH_NODE *op2){
    TAC *newtac = (TAC*) calloc(1, sizeof(TAC));

    newtac->type = type;
    newtac->res = res;
    newtac->op1 = op1;
    newtac->op2 = op2;

    newtac->prev = 0;
    newtac->next = 0;
    return newtac;
}
void tacPrint(TAC *tac){

    if(!tac)
        return;
    
    fprintf(stderr, "TAC(");

    switch(tac->type){
        case TAC_SYMBOL:
            fprintf(stderr, "TAC_SYMBOL");
            break;
        case TAC_SUM:
            fprintf(stderr, "TAC_SUM");
            break;
        case TAC_DEC:
            fprintf(stderr, "TAC_DEC");
            break;
        default:
            fprintf(stderr, "TAC_UNKNOWN");
            break;
    }
    fprintf(stderr, ", %s", (tac->res) ? tac->res->text : "0");
    fprintf(stderr, ", %s", (tac->op1) ? tac->op1->text : "0");
    fprintf(stderr, ", %s", (tac->op2) ? tac->op2->text : "0");
    fprintf(stderr, ");\n");
}
void tacPrintBackwards(TAC *tac){
    if(tac == NULL)
        return;
    tacPrintBackwards(tac->prev);
    tacPrint(tac);
}
TAC* tacJoin(TAC *l1, TAC *l2){
    TAC *point;

    if(l1 == NULL)
        return l2;
    if(l2 == NULL)
        return l1;

    for(point = l2; point->prev != 0; point = point->prev);

    point->prev = l1;
    return l2;
}

TAC* generateCode(AST *node){

}