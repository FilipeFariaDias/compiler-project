#ifndef TACS_HEADER
#define TACS_HEADER

#include "hash.h"
#include "ast.h"

#define TAC_SYMBOL  1
#define TAC_SUM     2
#define TAC_DEC     3

/* Não sei se todos esses defines são necessários, conferir
#define TAC_MOVE 4
#define TAC_MUL 5
#define TAC_DIV 6
#define TAC_GREAT 7
#define TAC_LESS 8
#define TAC_GE 9
#define TAC_LE 10
#define TAC_EQ 11
#define TAC_DIF 12
#define TAC_AND 13
#define TAC_OR 14
#define TAC_NOT 15
#define TAC_LABEL 16
#define TAC_BEGINFUN 17
#define TAC_ARGPUSH 18
#define TAC_ENDFUN 19
#define TAC_IFZ 20
#define TAC_JUMP 21
#define TAC_CALL 22
#define TAC_ARG 23
#define TAC_RET 24
#define TAC_PRINT 25
#define TAC_READ 26
#define TAC_VEC 27
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
