#include <stdlib.h>
#include <stdio.h>
#include "hash.h"
#include "ast.h"

AST* astCreate(int type, HASH_NODE* symbol, AST* son0, AST* son1, AST* son2, AST* son3){
    AST *node = calloc(1, sizeof(AST));
    node->type = type;
    node->symbol = symbol;
    node->sons[0] = son0;
    node->sons[1] = son1;
    node->sons[2] = son2;
    node->sons[3] = son3;

    return node;
}

void astPrint(int level, AST *node){
  if(node == NULL)
    return;

  int i;
  for(i = 0; i < level; i++)
    fprintf(stderr, "  ");

  fprintf(stderr, "AST( ");

  switch(node->type){
    case AST_LDEC: fprintf(stderr, "AST_LDEC"); break;
    case AST_DECVAR: fprintf(stderr, "AST_DECVAR"); break;
    case AST_DECVEC: fprintf(stderr, "AST_DECVEC"); break;
    case AST_DECFUNC: fprintf(stderr, "AST_DECFUNC"); break;
    case AST_TPCHAR: fprintf(stderr, "AST_TPCHAR"); break;
    case AST_TPINT: fprintf(stderr, "AST_TPINT"); break;
    case AST_TPFLOAT: fprintf(stderr, "AST_TPFLOAT"); break;
    case AST_SYMBOL: fprintf(stderr, "AST_SYMBOL"); break;
    case AST_VECSIZE: fprintf(stderr, "AST_VECSIZE"); break;
    case AST_VECINIT: fprintf(stderr, "AST_VECINIT"); break;
    case AST_VECELEMENTS: fprintf(stderr, "AST_VECELEMENTS"); break;
    case AST_PARAMINIT: fprintf(stderr, "AST_PARAMINIT"); break;
    case AST_PARAM: fprintf(stderr, "AST_PARAM"); break;
    case AST_BLOCK: fprintf(stderr, "AST_BLOCK"); break;
    case AST_CMDLSTINIT: fprintf(stderr, "AST_CMDLSTINIT"); break;
    case AST_CMDLST: fprintf(stderr, "AST_CMDLST"); break;
    case AST_ATTR: fprintf(stderr, "AST_ATTR"); break;
    case AST_VECATTR: fprintf(stderr, "AST_VECATTR"); break;
    case AST_READ: fprintf(stderr, "AST_READ"); break;
    case AST_READINDEX: fprintf(stderr, "AST_READINDEX"); break;
    case AST_PRINT: fprintf(stderr, "AST_PRINT"); break;
    case AST_RETURN: fprintf(stderr, "AST_RETURN"); break;
    case AST_WHILE: fprintf(stderr, "AST_WHILE"); break;
    case AST_IFELSE: fprintf(stderr, "AST_IFELSE"); break;
    case AST_ELSE: fprintf(stderr, "AST_ELSE"); break;
    case AST_VEC: fprintf(stderr, "AST_VEC"); break;
    case AST_FUNC: fprintf(stderr, "AST_FUNC"); break;
    case AST_SUM: fprintf(stderr, "AST_SUM"); break;
    case AST_DEC: fprintf(stderr, "AST_DEC"); break;
    case AST_DOT: fprintf(stderr, "AST_DOT"); break;
    case AST_DIV: fprintf(stderr, "AST_DIV"); break;
    case AST_LESS: fprintf(stderr, "AST_LESS"); break;
    case AST_GREAT: fprintf(stderr, "AST_GREAT"); break;
    case AST_EQ: fprintf(stderr, "AST_EQ"); break;
    case AST_GE: fprintf(stderr, "AST_GE"); break;
    case AST_LE: fprintf(stderr, "AST_LE"); break;
    case AST_DIF: fprintf(stderr, "AST_DIF"); break;
    case AST_AND: fprintf(stderr, "AST_AND"); break;
    case AST_OR: fprintf(stderr, "AST_OR"); break;
    case AST_NOT: fprintf(stderr, "AST_NOT"); break;
    case AST_PAREN: fprintf(stderr, "AST_PAREN"); break;
    case AST_ARGUMENT: fprintf(stderr, "AST_ARGUMENT"); break;
    case AST_PRINTLST: fprintf(stderr, "AST_PRINTLST");  break;
  }

  if(node->symbol != NULL)
    fprintf(stderr, ", %s )\n", node->symbol->text);
  else
    fprintf(stderr, " )\n");

  for(i = 0; i < MAX_SONS; i++)
    astPrint(level+1, node->sons[i]);
}

void decompileAST(AST *node, FILE *file){

  if(node == NULL)
    return;

  switch(node->type){
    case AST_LDEC :
      decompileAST(node->sons[0], file);
      decompileAST(node->sons[1], file);
      break;

    case AST_DECVAR :
      decompileAST(node->sons[0], file);
      fprintf(file, " %s = ", node->symbol->text);
      decompileAST(node->sons[1], file);
      fprintf(file, ";\n");
      break;

    case AST_DECVEC :
      decompileAST(node->sons[0], file);
      fprintf(file, " %s[", node->symbol->text);
      decompileAST(node->sons[1], file);
      fprintf(file, "]");      
      decompileAST(node->sons[2], file);
      fprintf(file, ";\n");
      break;

    case AST_DECFUNC :
      decompileAST(node->sons[0], file);
      fprintf(file, " %s(", node->symbol->text);
      decompileAST(node->sons[1], file);
      fprintf(file, ")");      
      decompileAST(node->sons[2], file);
      fprintf(file, ";\n");
      break;

    case AST_TPCHAR :
      fprintf(file, "char");
      break;

    case AST_TPINT :
      fprintf(file, "int");
      break;

    case AST_TPFLOAT :
      fprintf(file, "float");
      break;

    case AST_SYMBOL :
      fprintf(file, " %s ", node->symbol->text);
      break;

    case AST_VECINIT :
      fprintf(file, ": ");
      decompileAST(node->sons[0], file);
      fprintf(file, " ");
      decompileAST(node->sons[1], file);
      break;

    case AST_VECSIZE :
      fprintf(file, ": ");
      decompileAST(node->sons[0], file);
      fprintf(file, " ");
      decompileAST(node->sons[1], file);
      break;

    case AST_VECELEMENTS :
      decompileAST(node->sons[0], file);
      fprintf(file, " ");
      decompileAST(node->sons[1], file);
      break;

    case AST_PARAMINIT :
      decompileAST(node->sons[0], file);
      decompileAST(node->sons[1], file);
      break;

    case AST_PARAM :
      fprintf(file, ", ");
      decompileAST(node->sons[0], file);
      decompileAST(node->sons[1], file);
      break;

    case AST_BLOCK :
      fprintf(file, "{\n");
      decompileAST(node->sons[0], file);
      fprintf(file, "}");
      break;

    case AST_CMDLSTINIT :
      decompileAST(node->sons[0], file);
      decompileAST(node->sons[1], file);
      break;

    case AST_CMDLST :
      fprintf(file, ";\n");
      decompileAST(node->sons[0], file);
      decompileAST(node->sons[1], file);
      break;

    case AST_ATTR :
      fprintf(file, "%s = ", node->symbol->text);
      decompileAST(node->sons[0], file);
      break;

    case AST_VECATTR :
      fprintf(file, "%s[", node->symbol->text);
      decompileAST(node->sons[0], file);
      fprintf(file, "] = ");
      decompileAST(node->sons[1], file);
      break;

    case AST_READ :
      fprintf(file, "read %s", node->symbol->text);
      break;
    case AST_READINDEX :
      fprintf(file, "read index %s", node->symbol->text);
      fprintf(file, "[");
      decompileAST(node->sons[0],file); 
      fprintf(file,"]"); 	
      decompileAST(node->sons[1],file);
      break;

    case AST_PRINT :
      fprintf(file, "print ");
      decompileAST(node->sons[0], file);
      break;

    case AST_RETURN :
      fprintf(file, "return ");
      decompileAST(node->sons[0], file);
      break;

    case AST_WHILE :
      fprintf(file, "while(");
      decompileAST(node->sons[0], file);
      fprintf(file, ")");
      decompileAST(node->sons[1], file);
      break;

    case AST_IFELSE :
      fprintf(file,"if("); 
      decompileAST(node->sons[0],file); 
      fprintf(file," "); 
      fprintf(file,"cmd "); 
      decompileAST(node->sons[1],file); 
      fprintf(file,"else "); 
      decompileAST(node->sons[2],file); 
      break;

    case AST_ELSE :
      fprintf(file, "else ");
      decompileAST(node->sons[0], file);
      break;

    case AST_VEC :
      fprintf(file, "%s[", node->symbol->text);
      decompileAST(node->sons[0], file);
      fprintf(file, "]");
      break;

    case AST_FUNC :
      fprintf(file, "%s(", node->symbol->text);
      decompileAST(node->sons[0], file);
      fprintf(file, ")");
      break;

    case AST_SUM :
      decompileAST(node->sons[0], file);
      fprintf(file, " + ");
      decompileAST(node->sons[1], file);
      break;

    case AST_DEC :
      decompileAST(node->sons[0], file);
      fprintf(file, " - ");
      decompileAST(node->sons[1], file);
      break;

    case AST_DOT :
      decompileAST(node->sons[0], file);
      fprintf(file, " . ");
      decompileAST(node->sons[1], file);
      break;

    case AST_DIV :
      decompileAST(node->sons[0], file);
      fprintf(file, " / ");
      decompileAST(node->sons[1], file);
      break;

    case AST_LESS :
      decompileAST(node->sons[0], file);
      fprintf(file, " < ");
      decompileAST(node->sons[1], file);
      break;

    case AST_GREAT :
      decompileAST(node->sons[0], file);
      fprintf(file, " > ");
      decompileAST(node->sons[1], file);
      break;

    case AST_EQ :
      decompileAST(node->sons[0], file);
      fprintf(file, " == ");
      decompileAST(node->sons[1], file);
      break;

    case AST_GE :
      decompileAST(node->sons[0], file);
      fprintf(file, " >= ");
      decompileAST(node->sons[1], file);
      break;

    case AST_LE :
      decompileAST(node->sons[0], file);
      fprintf(file, " <= ");
      decompileAST(node->sons[1], file);
      break;

    case AST_DIF :
      decompileAST(node->sons[0], file);
      fprintf(file, " != ");
      decompileAST(node->sons[1], file);
      break;

    case AST_AND :
      decompileAST(node->sons[0], file);
      fprintf(file, " and ");
      decompileAST(node->sons[1], file);
      break;

    case AST_OR :
      decompileAST(node->sons[0], file);
      fprintf(file, " or ");
      decompileAST(node->sons[1], file);
      break;

    case AST_NOT :
      fprintf(file, "not ");
      decompileAST(node->sons[0], file);
      break;

    case AST_PAREN :
      fprintf(file, "(");
      decompileAST(node->sons[0], file);
      fprintf(file, ")");
      break;

    case AST_ARGUMENT :
      decompileAST(node->sons[0], file);
      decompileAST(node->sons[1], file);
      break;

    case AST_PRINTLST :
      fprintf(file, ", ");
      decompileAST(node->sons[0], file);
      decompileAST(node->sons[1], file);
      break;
  }
}
