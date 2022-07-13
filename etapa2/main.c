#include <stdio.h>
#include <stdlib.h>

extern FILE* yyin;
extern FILE* file();
extern int isRunning();
extern int getLineNumber();
extern int yyparse();
extern void initMe();
extern void hashPrint();

int main(int argc, char *argv[]){

    int token;
	
	if(argc < 2){
		fprintf(stderr,"Call: ./etapa2 nomearquivo\n");
		return 1;
	}
	if(!(file(argv[1]))){
		fprintf(stderr,"Cannot open file %s\n",argv[1]);
		return 2;
	}
	
	initMe();
	
	yyparse();
	
	hashPrint();

	fprintf(stderr, "OK\n");

 return 0; 
}

