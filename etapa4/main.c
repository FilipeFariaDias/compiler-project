#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "hash.h"

extern FILE* file();
extern AST * astFinal;
extern int yyparse();
extern void initMe();

int main(int argc, char *argv[]){

    int token;
	FILE *output;
	
	if(argc < 3){
		fprintf(stderr,"Call: ./etapa3 input.txt output.txt \n");
		exit(1);
	}
	if(!(file(argv[1]))){
		fprintf(stderr,"Cannot open file %s\n",argv[1]);
		exit(2);
	}
  
	if(!(output = fopen(argv[2], "w+"))){
			fprintf(stderr,"Cannot open file %s\n",argv[2]);
			exit(2);
		}
	
	initMe();
	
	yyparse();
  
	//hashPrint();
	
	decompileAST(astFinal, output);

	if(getSemanticErrors() > 0){
		fprintf(stderr, "Compilation Failure! Semantic Errors\n");
		exit(4)
	}

 	fprintf(stderr, "OK \n");

	fclose(output);

 	exit(0); 
}
