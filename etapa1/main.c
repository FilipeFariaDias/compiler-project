#include <stdio.h>
#include <stdlib.h>
#include "tokens.h"
#include "hash.h"

int yylex();
extern char *yytext;
extern FILE *yyin;

int getLineNumber();
int isRunning(void);
void initMe(void);


int main(int argc, char** argv){

	int tok;

	if (argc < 2){
		printf("call: ./etapa1 input.txt\n");
		exit(1);
    }
  	if (0 == (yyin = fopen(argv[1], "r"))){
		printf("Cannot open file %s... \n",argv[1]);
		exit(1);
    }

	initMe();
	while(isRunning()){

		tok = yylex();
		if (!isRunning())
      		break;

		switch(tok){
			case KW_CHAR: fprintf(stderr, "KW_CHAR na linha %d. - tok: %d \n", getLineNumber(), tok); break;
			case KW_INT:  fprintf(stderr, "KW_INT na linha %d. - tok: %d \n", getLineNumber(), tok); break;
      			case KW_FLOAT:fprintf(stderr, "KW_FLOAT na linha %d. - tok: %d \n", getLineNumber(), tok); break;
      			case KW_IF:   fprintf(stderr, "KW_IF na linha %d. - tok: %d \n", getLineNumber(), tok); break;
     			case KW_ELSE: fprintf(stderr, "KW_ELSE na linha %d. - tok: %d \n", getLineNumber(), tok); break;
			case KW_WHILE: fprintf(stderr, "KW_WHILE na linha %d. - tok: %d \n", getLineNumber(), tok); break;
      			case KW_READ: fprintf(stderr, "KW_READ na linha %d. - tok: %d \n", getLineNumber(), tok); break;
      			case KW_PRINT: fprintf(stderr, "KW_PRINT na linha %d. - tok: %d \n", getLineNumber(), tok); break;
     			case KW_RETURN: fprintf(stderr, "KW_RETURN na linha %d. - tok: %d \n", getLineNumber(), tok); break;
      			case ASSIGNMENT: fprintf(stderr, "ASSIGNMENT na linha %d. - tok: %d \n", getLineNumber(), tok); break;
      			case OPERATOR_LE: fprintf(stderr, "OPERATOR_LE na linha %d. - tok: %d \n", getLineNumber(), tok); break;
      			case OPERATOR_GE: fprintf(stderr, "OPERATOR_GE na linha %d. - tok: %d \n", getLineNumber(), tok); break;
      			case OPERATOR_EQ: fprintf(stderr, "OPERATOR_EQ na linha %d. - tok: %d \n", getLineNumber(), tok); break;
      			case OPERATOR_DIF: fprintf(stderr, "OPERATOR_DIF na linha %d. - tok: %d \n", getLineNumber(), tok); break;
      			case TK_IDENTIFIER: fprintf(stderr, "TK_IDENTIFIER na linha %d. - tok: %d \n", getLineNumber(), tok); break;
      			case LIT_INTEGER: fprintf(stderr, "LIT_INTEGER na linha %d. - tok: %d \n", getLineNumber(), tok); break;
      			case LIT_FLOAT: fprintf(stderr, "LIT_FLOAT na linha %d. - tok: %d \n", getLineNumber(), tok); break;
			case LIT_CHAR: fprintf(stderr, "LIT_CHAR na linha %d. - tok: %d \n", getLineNumber(), tok); break;
      			case LIT_STRING: fprintf(stderr, "LIT_STRING na linha %d. - tok: %d \n", getLineNumber(), tok); break;
      			case TOKEN_ERROR: fprintf(stderr, "TOKEN_ERROR na linha %d. - tok: %d \n", getLineNumber(), tok); break;
			default: fprintf(stderr, "Caracter especial na linha %d. - tok: %d \n", getLineNumber(), tok); break;
		}
	}

	hashPrint();

	return 0;
}
