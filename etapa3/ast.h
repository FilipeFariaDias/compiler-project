#ifndef AST_HEADER
#define AST_HEADER

#include "hash.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_SONS 4
#define AST_LDEC 1
#define AST_DECVAR 2
#define AST_DECVEC 3
#define AST_DECFUNC 4
#define AST_TPBYTE 5
#define AST_TPINT 6
#define AST_TPFLOAT 7
#define AST_SYMBOL 8
#define AST_VECINIT 9
#define AST_VECELEMENTS 10
#define AST_PARAMINIT 11
#define AST_PARAMLST 12
#define AST_PARAM 13
#define AST_BLOCK 14
#define AST_CMDLSTINIT 15
#define AST_CMDLST 16
#define AST_ATTR 17
#define AST_VECATTR 18
#define AST_READ 19
#define AST_PRINT 20
#define AST_RETURN 21
#define AST_IF 22
#define AST_LOOP 23
#define AST_IFELSE 24
#define AST_LEAP 25
#define AST_VEC 26
#define AST_FUNC 27
#define AST_SUM 28
#define AST_DEC 29
#define AST_MUL 30
#define AST_DIV 31
#define AST_LESS 32
#define AST_GREAT 33
#define AST_EQ 34
#define AST_GE 35
#define AST_LE 36
#define AST_DIF 37
#define AST_AND 38
#define AST_OR 39
#define AST_NOT 40
#define AST_PAREN 41
#define AST_ARGLSTINIT 42
#define AST_ARGLST 43
#define AST_PRINTLSTINIT 44
#define AST_PRINTLST 45

typedef struct astnode
{
    int type;
    HASH_NODE *symbol;
    struct astnode *sons[MAX_SONS];
} AST;


AST* astCreate(int type, HASH_NODE* symbol, AST* son0, AST* son1, AST* son2, AST* son3);

void astPrint(int level, AST *node);

void decompileAST(AST *root, FILE *file);

#endif
