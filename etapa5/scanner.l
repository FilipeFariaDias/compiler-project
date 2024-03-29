%{
    #include "hash.h"
    #include "ast.h"
    #include "y.tab.h"
    
    int lineNumber = 1;
    int Running = 1;
%}

special_chars [\-\;\(\)\[\]\{\}\+\.\/\<\>\&\|\~]
identifier [A-Za-z_]+
char \'(\\.|[^\\])\'
string \"([^"\\\n]|\\.)*\"
float [0-9]*\,[0-9]+
integer [0-9]+

%x COMMENT

%%
 /* Reserved words */
"char" { return KW_CHAR; }
"int" { return KW_INT; }
"float" { return KW_FLOAT; }
"if" { return KW_IF; }
"else" { return KW_ELSE; }
"while" { return KW_WHILE; }
"read" { return KW_READ; }
"print" { return KW_PRINT; }
"return" { return KW_RETURN; }

 /* Literals */
{char} {yylval.symbol = hashInsert(yytext, SYMBOL_LIT_CHAR); return LIT_CHAR; }
{string} {yylval.symbol = hashInsert(yytext, SYMBOL_LIT_STRING); return LIT_STRING; }
{float} {yylval.symbol = hashInsert(yytext, SYMBOL_LIT_FLOAT); return LIT_FLOAT; }
{integer} {yylval.symbol = hashInsert(yytext, SYMBOL_LIT_INTEGER); return LIT_INTEGER; }

 /* Composite operators */
"<-" { return ASSIGNMENT; }
"<=" { return OPERATOR_LE; }
">=" { return OPERATOR_GE; }
"==" { return OPERATOR_EQ; }
"!=" { return OPERATOR_DIF; }

 /* Special characters */
{special_chars} { return yytext[0]; }

 /* Identifier */
{identifier} {yylval.symbol = hashInsert(yytext, SYMBOL_IDENTIFIER); return TK_IDENTIFIER; }

 /* White space and tab*/
[ \t]

 /* Count line number */
"\n" { ++lineNumber; }

 /* Comments */
\#\#.*

\\\# { BEGIN(COMMENT); }
<COMMENT>.
<COMMENT>"\n" { ++lineNumber; }
<COMMENT>\#\\ { BEGIN(INITIAL); }

 /* Error if no pattern is matched */
. { return TOKEN_ERROR; }


%%

void initMe(void){
    hashInit();
}

FILE* file(char* filename){
    yyin = fopen(filename,"r");
    return yyin;
}

int getLineNumber(void){
    return lineNumber;
}

int isRunning(void){
    return Running;
}

int yywrap(){
    Running = 0;
    return 1;
}
