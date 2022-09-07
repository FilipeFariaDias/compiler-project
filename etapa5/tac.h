#ifndef TACS_HEADER
#define TACS_HEADER

#include "hash.h"
#include "ast.h"

#define TAC_SYMBOL  1
#define TAC_SUM     2
#define TAC_DEC     3
#define TAC_DOT     4
#define TAC_DIV     5
#define TAC_GREAT   6
#define TAC_LESS    7
#define TAC_GE      8
#define TAC_LE      9
#define TAC_EQ      10
#define TAC_DIF     11
#define TAC_AND     12
#define TAC_OR      13
#define TAC_NOT     14
#define TAC_LABEL   15
#define TAC_JFZ     19
#define TAC_JUMP    20
#define TAC_RET     23
#define TAC_PRINT   24
#define TAC_READ    25

/* Não sei se todos esses defines são necessários, conferir
#define TAC_BEGINFUN 16
#define TAC_ARGPUSH 17
#define TAC_ENDFUN 18
#define TAC_CALL 21
#define TAC_ARG 22
#define TAC_VEC 26
*/

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
