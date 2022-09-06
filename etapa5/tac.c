#include <stdio.h>
#include <stdlib.h>
#include "tac.h"

TAC *binaryOperation(int opcode, TAC *code0, TAC *code1);

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
    int i;
    TAC *result = 0;
    TAC *code[MAX_SONS];

    if(!node)
        return 0;

    for(i = 0; i < MAX_SONS; i++)
        code[i] = generateCode(node->sons[i]);

    switch(node->type){
        case AST_SYMBOL:
            result = tacCreate(TAC_SYMBOL, node->symbol, 0, 0);
            break;
        case AST_SUM:
            result = binaryOperation(TAC_SUM, code[0], code[1]);
            break;
	    case AST_DEC:
            result = binaryOperation(TAC_DEC, code[0], code[1]);
            break;
	    case AST_DOT:
            result = binaryOperation(TAC_DOT, code[0], code[1]);
            break;
	    case AST_DIV:
            result = binaryOperation(TAC_DIV, code[0], code[1]);
            break;
	    case AST_LESS:
            result = binaryOperation(TAC_LESS, code[0], code[1]);
            break;
	    case AST_GREAT:
            result = binaryOperation(TAC_GREAT, code[0], code[1]);
            break;
	    case AST_EQ:
            result = binaryOperation(TAC_EQ, code[0], code[1]);
            break;
	    case AST_GE:
            result = binaryOperation(TAC_GE, code[0], code[1]);
            break;
	    case AST_LE:
            result = binaryOperation(TAC_LE, code[0], code[1]);
            break;
	    case AST_DIF:
            result = binaryOperation(TAC_DIF, code[0], code[1]);
            break;
	    case AST_AND:
            result = binaryOperation(TAC_AND, code[0], code[1]);
            break;
	    case AST_OR:
            result = binaryOperation(TAC_OR, code[0], code[1]);
            break;
	    case AST_NOT:
            result = tacJoin(code[0], tacCreate(TAC_NOT, makeTemp(), code[0] ? code[0]->res : 0, 0));
            break;
        default:
            result = tacJoin(code[0], tacJoin(code[1], tacJoin(code[2], code[3])));
            break;
    }

    return result;
}

TAC *binaryOperation(int opcode, TAC *code0, TAC *code1){

    HASH_NODE *op1 = code0->res ? code0->res : 0;
    HASH_NODE *op2 = code1->res ? code1->res : 0;

    return tacJoin(tacJoin(code0, code1), tacCreate(opcode, makeTemp(), op1, op2));
}