%{

   #include <stdio.h>
   #include <stdlib.h>
   #include <string.h>
   #include "hash.h"
   #include "astree.h"
	
   int yylex();
   int yyerror(char *message);
   extern int getLineNumber();

%}

%token KW_CHAR
%token KW_INT
%token KW_FLOAT

%token KW_IF
%token KW_ELSE
%token KW_WHILE
%token KW_READ
%token KW_PRINT
%token KW_RETURN

%token ASSIGNMENT
%token OPERATOR_LE
%token OPERATOR_GE
%token OPERATOR_EQ
%token OPERATOR_DIF

%token TK_IDENTIFIER

%token LIT_INTEGER
%token LIT_FLOAT
%token LIT_CHAR
%token LIT_STRING

%token TOKEN_ERROR

%left OPERATOR_LE OPERATOR_GE OPERATOR_EQ OPERATOR_DIF '<' '>'
%left '+' '-' 
%left '.' '/'
%left '&' '|' '~'

%start programa

%%

programa: lista_declaracoes
	;

lista_declaracoes: declaracoes lista_declaracoes
	|
	;
		
declaracoes: tipo TK_IDENTIFIER '(' literal ')' ';'		
	| tipo TK_IDENTIFIER '[' LIT_INTEGER ']' inicia_vetor ';'
	| tipo TK_IDENTIFIER '(' parametro ')' bloco
	; 

tipo: KW_CHAR	     	
	| KW_INT
	| KW_FLOAT
	;

literal: LIT_INTEGER 
	| LIT_FLOAT
	| LIT_CHAR
	;
			
inicia_vetor: ' '
	| literal elemento_vetor
	|
	;
			
elemento_vetor: 
	| literal elemento_vetor
	;
			
parametro: ' ' param parametro
	| param parametro
	|
	;

param: tipo TK_IDENTIFIER
	;

bloco: '{' comando '}'
	;
			
comando: cmd comando_fim
	;

comando_fim:
	| ';' cmd comando_fim
	;

cmd:
	| bloco
	| TK_IDENTIFIER ASSIGNMENT exp
	| TK_IDENTIFIER '[' exp ']' ASSIGNMENT exp
	| cmdRead
	| KW_PRINT l_print
	| KW_RETURN exp
	| KW_IF '(' exp ')' cmd else cmd 
	| KW_WHILE '(' exp ')' cmd
	;

else:
	| KW_ELSE cmd
	;

cmdRead: KW_READ TK_IDENTIFIER
	| KW_READ TK_IDENTIFIER '[' exp ']'
	;

exp: TK_IDENTIFIER	
	| TK_IDENTIFIER '[' exp ']'
	| TK_IDENTIFIER '(' argumento ')'
	| literal
	| exp '+' exp
	| exp '-' exp
	| exp '.' exp
	| exp '/' exp
	| exp '<' exp
	| exp '>' exp 
	| exp OPERATOR_EQ exp
	| exp OPERATOR_GE exp
	| exp OPERATOR_LE exp
	| exp OPERATOR_DIF exp
	| exp '&' exp
	| exp '|' exp
	| exp '~' exp
	| '(' exp ')'
	;
			
argumento: exp argumento
	| ' ' argumento
	|
	;
			
l_print: LIT_STRING l_print
	| exp l_print
	| ' ' l_print
	|
	;

			
%%

int yyerror(char *err){

	fprintf(stderr, "Erro na linha %d\n", getLineNumber());
	exit(3);
}			
