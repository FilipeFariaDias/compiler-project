#ifndef TACS_HEADER
#define TACS_HEADER

#include "hash.h"
#include "ast.h"

typedef struct tac_node{
    int type;
    HASH_NODE *res;
    HASH_NODE *op1;
    HASH_NODE *op2;
    struct tac_node *prev;
    struct tac_node *next;
} TAC;


TAC* tacCreate(int type, HASH_NODE *resultado, HASH_NODE *op1, HASH_NODE *op2);
void tacPrint(TAC *tac);
void tacPrintBackwards(TAC *tac);
TAC* tacJoin(TAC *l1, TAC *l2);

TAC* generateCode(AST *node);

#endif