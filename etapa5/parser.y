%{
	
   #include "hash.h"
   #include "ast.h"
   #include "semantic.h"
   #include "tac.h"
	
   AST *astFinal;
   int yyerror();
   int yylex();
   extern int getLineNumber();

%}

%union
{
	HASH_NODE *symbol;
	AST *ast;
}

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

%token<symbol> TK_IDENTIFIER

%token<symbol> LIT_INTEGER
%token<symbol> LIT_FLOAT
%token<symbol> LIT_CHAR
%token<symbol> LIT_STRING

%token TOKEN_ERROR

%type<ast> programa
%type<ast> lista_declaracoes
%type<ast> declaracoes
%type<ast> tipo
%type<ast> literal
%type<ast> inicia_vetor
%type<ast> elemento_vetor
%type<ast> parametro
%type<ast> param
%type<ast> bloco
%type<ast> comando
%type<ast> comando_fim
%type<ast> cmd
%type<ast> else
%type<ast> cmdRead
%type<ast> exp
%type<ast> argumento
%type<ast> l_print


%left OPERATOR_LE OPERATOR_GE OPERATOR_EQ OPERATOR_DIF '<' '>'
%left '+' '-' 
%left '.' '/'
%left '&' '|' '~'

%start programa

%%

programa: lista_declaracoes						{ 
													astFinal = $$; 
													astPrint(0, astFinal); 
													checkSemanticAnalysis(astFinal);
													tacPrintBackwards(generateCode($1));
												}
	;

lista_declaracoes: declaracoes lista_declaracoes			 { $$ = astCreate(AST_LDEC, 0, $1, $2, 0, 0); }
	|														{ $$ = 0; }
	;
		
declaracoes: tipo TK_IDENTIFIER '(' literal ')' ';'		{ $$ = astCreate(AST_DECVAR, $2, $1, $4, 0, 0); }
	| tipo TK_IDENTIFIER '[' LIT_INTEGER ']' inicia_vetor ';' 		{ $$ = astCreate(AST_DECVEC, $2, $1, astCreate(AST_VECSIZE, $4, 0, 0, 0, 0), $6, 0); }
	| tipo TK_IDENTIFIER '(' parametro ')' bloco				{ $$ = astCreate(AST_DECFUNC, $2, $1, $4, $6, 0); }
	; 

tipo: KW_CHAR	     		{ $$ = astCreate(AST_TPCHAR, 0, 0, 0, 0, 0); }
	| KW_INT				{ $$ = astCreate(AST_TPINT, 0, 0, 0, 0, 0); }
	| KW_FLOAT				{ $$ = astCreate(AST_TPFLOAT, 0, 0, 0, 0, 0); }
	;

literal: LIT_INTEGER 		{ $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); }
	| LIT_FLOAT				{ $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); }
	| LIT_CHAR				{ $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); }
	;
			
inicia_vetor: literal elemento_vetor 		{ $$ = astCreate(AST_VECINIT, 0, $1, $2, 0, 0); }
	|										{ $$ = 0; }
	;	
			
elemento_vetor: literal elemento_vetor 		{ $$ = astCreate(AST_VECELEMENTS, 0, $1, $2, 0, 0); }
	| 										{ $$ = 0; }
	;
			
parametro: param parametro					{ $$ = astCreate(AST_PARAMINIT, 0, $1, $2, 0, 0); }
	|										{ $$ = 0; }
	;

param: tipo TK_IDENTIFIER					{ $$ = astCreate(AST_PARAM, $2, $1, 0, 0, 0); }
	;

bloco: '{' comando '}'						{ $$ = astCreate(AST_BLOCK, 0, $2, 0, 0, 0); }
	;
			
comando: cmd comando_fim					{ $$ = astCreate(AST_CMDLSTINIT, 0, $1, $2, 0, 0); }
	;

comando_fim: ';' cmd comando_fim			{ $$ = astCreate(AST_CMDLST, 0, $2, $3, 0, 0); }
	| 										{ $$ = 0; }
	;

cmd: TK_IDENTIFIER ASSIGNMENT exp					{ $$ = astCreate(AST_ATTR, $1, $3, 0, 0, 0); }
	| TK_IDENTIFIER '[' exp ']' ASSIGNMENT exp		{ $$ = astCreate(AST_VECATTR, $1, $3, $6, 0, 0); }
	| cmdRead										{ $$ = $1; }
	| KW_PRINT l_print								{ $$ = astCreate(AST_PRINT, 0, $2, 0, 0, 0); }
	| KW_RETURN exp									{ $$ = astCreate(AST_RETURN, 0, $2, 0, 0, 0); }
	| KW_IF '(' exp ')' cmd 						{ $$ = astCreate(AST_IF, 0, $3, $5, 0, 0); }
	| KW_IF '(' exp ')' cmd else cmd 				{ $$ = astCreate(AST_IFELSE, 0, $3, $5, $6, $7); }
	| KW_WHILE '(' exp ')' cmd						{ $$ = astCreate(AST_WHILE, 0, $3, $5, 0, 0); }
	| bloco											{ $$ = $1; }
	|												{ $$ = 0; }
	;

else: KW_ELSE cmd									{ $$ = astCreate(AST_ELSE, 0, $2, 0, 0, 0); }
	;

cmdRead: KW_READ TK_IDENTIFIER						{ $$ = astCreate(AST_READ, $2, 0, 0, 0, 0); }		
	| KW_READ TK_IDENTIFIER '[' exp ']'				{ $$ = astCreate(AST_READINDEX, $2, $4, 0, 0, 0); }	
	;

exp: TK_IDENTIFIER									{ $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); }	
	| TK_IDENTIFIER '[' exp ']'						{ $$ = astCreate(AST_VEC, $1, $3, 0, 0, 0); }	
	| TK_IDENTIFIER '(' argumento ')'				{ $$ = astCreate(AST_FUNC, $1, $3, 0, 0, 0); }	
	| literal										{ $$ = $1; }
	| exp '+' exp									{ $$ = astCreate(AST_SUM, 0, $1, $3, 0, 0); }	
	| exp '-' exp									{ $$ = astCreate(AST_DEC, 0, $1, $3, 0, 0); }
	| exp '.' exp									{ $$ = astCreate(AST_DOT, 0, $1, $3, 0, 0); }
	| exp '/' exp									{ $$ = astCreate(AST_DIV, 0, $1, $3, 0, 0); }
	| exp '<' exp									{ $$ = astCreate(AST_LESS, 0, $1, $3, 0, 0); }
	| exp '>' exp 									{ $$ = astCreate(AST_GREAT, 0, $1, $3, 0, 0); }
	| exp OPERATOR_EQ exp							{ $$ = astCreate(AST_EQ, 0, $1, $3, 0, 0); }
	| exp OPERATOR_GE exp							{ $$ = astCreate(AST_GE, 0, $1, $3, 0, 0); }
	| exp OPERATOR_LE exp							{ $$ = astCreate(AST_LE, 0, $1, $3, 0, 0); }
	| exp OPERATOR_DIF exp							{ $$ = astCreate(AST_DIF, 0, $1, $3, 0, 0); }
	| exp '&' exp									{ $$ = astCreate(AST_AND, 0, $1, $3, 0, 0); }
	| exp '|' exp									{ $$ = astCreate(AST_OR, 0, $1, $3, 0, 0); }
	| exp '~' exp									{ $$ = astCreate(AST_NOT, 0, $1, $3, 0, 0); }
	| '(' exp ')'									{ $$ = astCreate(AST_PAREN, 0, $2, 0, 0, 0); }
	;
			
argumento: exp argumento							{ $$ = astCreate(AST_ARGUMENT, 0, $1, $2, 0, 0); }
	|												{ $$ = 0; }
	;

l_print: LIT_STRING l_print							{ $$ = astCreate(AST_SYMBOL, $1, $2, 0, 0, 0); }
	| exp l_print									{ $$ = astCreate(AST_PRINTLST, 0, $1, $2, 0, 0); }
	|												{ $$ = 0; }
	;

			
%%

int yyerror()
{
	fprintf(stderr, "Erro na linha %d. \n", getLineNumber());
	exit(3);
}			

