#include <stdio.h>
#include <stdlib.h>
#include "tac.h"

TAC *binaryOperation(int opcode, TAC *code0, TAC *code1);
TAC *createIf(TAC *code0, TAC *code1);
TAC *createIfElse(TAC *code0, TAC *code1, TAC *code2);
TAC *createWhile(TAC *code0, TAC *code1);

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
        case TAC_DOT:
            fprintf(stderr, "TAC_DOT");
            break;
        case TAC_DIV:
            fprintf(stderr, "TAC_DIV");
            break;
        case TAC_GREAT:
            fprintf(stderr, "TAC_GREAT");
            break;
        case TAC_LESS:
            fprintf(stderr, "TAC_LESS");
            break;
        case TAC_GE:
            fprintf(stderr, "TAC_GE");
            break;
        case TAC_LE:
            fprintf(stderr, "TAC_LE");
            break;
        case TAC_EQ:
            fprintf(stderr, "TAC_EQ");
            break;
        case TAC_DIF:
            fprintf(stderr, "TAC_DIF");
            break;
        case TAC_AND:
            fprintf(stderr, "TAC_AND");
            break;
        case TAC_OR:
            fprintf(stderr, "TAC_OR");
            break;
        case TAC_NOT:
            fprintf(stderr, "TAC_NOT");
            break;
        case TAC_LABEL:
            fprintf(stderr, "TAC_LABEL");
            break;
        case TAC_JFZ:
            fprintf(stderr, "TAC_JFZ");
            break;
        case TAC_JUMP:
            fprintf(stderr, "TAC_JUMP");
            break;
        case TAC_RET:
            fprintf(stderr, "TAC_RET");
            break;
        case TAC_PRINT:
            fprintf(stderr, "TAC_PRINT");
            break;
        case TAC_READ:
            fprintf(stderr, "TAC_READ");
            break;
        case TAC_FUNC:
            fprintf(stderr, "TAC_FUNC");
            break;
        case TAC_ARGS:
            fprintf(stderr, "TAC_ARGS");
            break;
        case TAC_VEC:
            fprintf(stderr, "TAC_VEC");
            break;
        case TAC_VECCOPY:
            fprintf(stderr, "TAC_VECCOPY");
            break;
        case TAC_COPY:
            fprintf(stderr, "TAC_COPY");
            break;
        case TAC_INITFUN:
            fprintf(stderr, "TAC_INITFUN");
            break;
        case TAC_PARAMFUN:
            fprintf(stderr, "TAC_PARAMFUN");
            break;
        case TAC_DECVEC:
            fprintf(stderr, "TAC_DECVEC");
            break;
        case TAC_VECELEM:
            fprintf(stderr, "TAC_VECELEM");
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
        case AST_PRINT:
            result = code[0];
            break;
        case AST_PRINTLST:
            result = tacJoin(tacJoin(code[0], tacCreate(TAC_PRINT, code[0] ? code[0]->res : 0, 0, 0)), code[1]);
            break;
        case AST_READ:
            result = tacCreate(TAC_READ, node->symbol, 0, 0);
            break;
        case AST_RETURN:
            result = tacJoin(code[0], tacCreate(TAC_RET, code[0] ? code[0]->res : 0, 0, 0));
            break;
        case AST_IF:
            result = createIf(code[0], code[1]);
            break;
        case AST_IFELSE:
            result = createIfElse(code[0], code[1], code[2]);
            break;
        case AST_WHILE:
            result = createWhile(code[0], code[1]);
            break;
        case AST_ATTR:
            result = tacJoin(code[0],tacCreate(TAC_COPY, node->symbol, code[0] ? code[0]->res : 0, 0));
            break;
        case AST_FUNC:
            result = tacJoin(code[0], tacCreate(TAC_FUNC, makeTemp(), node->symbol, 0));
            break;
        case AST_ARGUMENT:
            result = tacJoin(tacJoin(code[0], tacCreate(TAC_ARGS, code[0] ? code[0]->res : 0, 0, 0)), code[1]);
            break;
        case AST_VEC:
            result = tacJoin(code[0], tacCreate(TAC_VEC, makeTemp(), node->symbol, code[0] ? code[0]->res : 0));
            break;
        case AST_VECATTR:
            result = tacJoin(tacJoin(code[0], code[1]), tacCreate(TAC_VECCOPY, node->symbol, code[1] ? code[1]->res : 0, code[0] ? code[0]->res : 0)); 
            break;
        case AST_DECVAR:
            result = tacJoin(code[1], tacCreate(TAC_COPY, node->symbol, code[1] ? code[1]->res : 0, 0));
            break;
        case AST_DECFUNC:
            result = tacJoin(tacJoin(tacJoin(tacJoin(tacCreate(TAC_INITFUN, tacCreate(TAC_SYMBOL, node->symbol, 0, 0)->res, 0, 0), code[0]), code[1]), code[2]), tacCreate(TAC_ENDFUN, tacCreate(TAC_SYMBOL, node->symbol, 0, 0)->res, 0, 0));
            break;
        case AST_PARAMINIT:
        case AST_PARAM:
            result = tacJoin(code[3], tacJoin(tacCreate(TAC_PARAMFUN, node->symbol, 0, 0), code[1]));
            break;
        case AST_DECVEC:
            result = tacJoin(tacCreate(TAC_DECVEC, node->symbol, code[2] ? code[2]->res : 0, 0), code[2]); 
            break;
        case AST_VECINIT:
        case AST_VECELEMENTS:
            result = tacJoin(tacCreate(TAC_VECELEM, code[0] ? code[0]->res : 0, 0, 0), code[1]);
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

TAC *createIf(TAC *code0, TAC *code1){
    TAC *jumptac = 0;
    TAC *labeltac = 0;
    HASH_NODE *newlabel = 0;

    newlabel = makeLabel();

    jumptac = tacCreate(TAC_JFZ, newlabel, code0->res, 0);
    jumptac->prev = code0;
    labeltac = tacCreate(TAC_LABEL, newlabel, 0, 0);
    labeltac->prev = code1;

    return tacJoin(jumptac, labeltac);
}

TAC *createIfElse(TAC *code0, TAC *code1, TAC *code2){
    HASH_NODE *labelIf = makeLabel();
    HASH_NODE *labeElse = makeLabel();
    
    TAC *tacjfalse = tacCreate(TAC_JFZ, labelIf, code0 ? code0->res : 0, 0);
    TAC *tacjmp = tacCreate(TAC_JUMP, labeElse, 0, 0);
    
    TAC *tacLabelIf = tacCreate(TAC_LABEL, labelIf, 0, 0);
    TAC *tacLabelElse = tacCreate(TAC_LABEL, labeElse, 0, 0);

    return tacJoin(code0, tacJoin(tacjfalse, tacJoin(code1, tacJoin(tacjmp, tacJoin(tacLabelIf, tacJoin(code2, tacLabelElse))))));
}

TAC *createWhile(TAC *code0, TAC *code1){
    HASH_NODE *labelStartLoop = makeLabel();
    HASH_NODE *labelEndLoop = makeLabel();
    
    TAC *tacEnd = tacCreate(TAC_JFZ, labelEndLoop, code0 ? code0->res : 0, 0);
    TAC *tacStart = tacCreate(TAC_JUMP, labelStartLoop, 0, 0);
    
    TAC *tacLabelStartLoop = tacCreate(TAC_LABEL, labelStartLoop, 0, 0);
    TAC *tacLabelEndLoop = tacCreate(TAC_LABEL, labelEndLoop, 0, 0);

    return tacJoin(tacLabelStartLoop, tacJoin(code0, tacJoin(tacEnd, tacJoin(code1, tacJoin(tacStart, tacLabelEndLoop)))));
}
