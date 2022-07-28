#include <stdio.h>
#include <stdlib.h>
#include "astree.h"

extern FILE* yyin;
extern FILE* file();
extern int isRunning();
extern int getLineNumber();
extern int yyparse();
extern void initMe();
extern void hashPrint();
extern astree_node * getAST();

int main(int argc, char *argv[]){

    int token;
	
	if(argc < 3){
		fprintf(stderr,"Call: ./etapa2 nomearquivo\n");
		return 1;
	}
	if(!(file(argv[1]))){
		fprintf(stderr,"Cannot open file %s\n",argv[1]);
		return 2;
	}
  
  if(!(out = fopen(argv[2], "w+"))){
		fprintf(stderr,"Cannot open file %s\n",argv[2]);
		return 2;
	}
	
	initMe();
	
	yyparse();
  
	//hashPrint();
	
  fprintf(stderr, "OK \n");

	fprintf(stderr, "Not OK \n");

	uncompileAST(getAST(), out);

	fclose(out);

 return 0; 
}