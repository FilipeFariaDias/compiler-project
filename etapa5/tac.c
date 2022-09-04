#include <stdio.h>
#include <stdlib.h>
#include "tac.h"


TAC* tacCreate(int type, HASH_NODE *resultado, HASH_NODE *op1, HASH_NODE *op2){

}
void tacPrint(TAC *tac){

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