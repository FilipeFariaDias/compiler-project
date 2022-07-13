%{
    #include <stdio.h>
    #include "tokens.h"
    #include "hash.c"

    int lineNumber = 1;
    int Running = 1;

%}

special_chars [\-\;\(\)\[\]\{\}\+\.\/\<\>\&\|\~]
identifier [A-Za-z_]+
char \'(\\.|[^\\])\'
string \"([^"\\\n]|\\.)*\"
float [+|-]?[0-9]*\,[0-9]+
integer [+|-]?[0-9]+

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
{char} { hashInsert(yytext, LIT_CHAR); return LIT_CHAR; }
{string} { hashInsert(yytext, LIT_STRING); return LIT_STRING; }
{float} { hashInsert(yytext, LIT_FLOAT); return LIT_FLOAT; }
{integer} { hashInsert(yytext, LIT_INTEGER); return LIT_INTEGER; }

 /* Composite operators */
"<-" { return ASSIGNMENT; }
"<=" { return OPERATOR_LE; }
">=" { return OPERATOR_GE; }
"==" { return OPERATOR_EQ; }
"!=" { return OPERATOR_DIF; }

 /* Special characters */
{special_chars} { return yytext[0]; }

 /* Idenifier */
{identifier} { hashInsert(yytext, TK_IDENTIFIER); return TK_IDENTIFIER; }

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

#include "main.c"

int getLineNumber(void){
    return lineNumber;
}

int isRunning(void){
    return Running;
}


void initMe(void){
    hashInit();
}

int yywrap(){
    Running = 0;
    return 1;
}


