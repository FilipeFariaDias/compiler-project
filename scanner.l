%{
    #include <stdio.h>
    #include "hash.c"
    #include "tokens.h"

    int lineNumber = 1;
    int Running = 1;

%}

%%

"char"          { return KW_CHAR; }
"int"           { return KW_INT; }
"float"         { return KW_FLOAT; }
"if"            { return KW_IF; }
"else"          { return KW_ELSE; }
"while"         { return KW_WHILE; }
"read"          { return KW_READ; }
"print"         { return KW_PRINT; }
"return"        { return KW_RETURN; }


"<-"          { return ASSIGNMENT; }
"<="          { return OPERATOR_LE; }
">="          { return OPERATOR_GE; }
"=="          { return OPERATOR_EQ; }
"!="          { return OPERATOR_DIF; }



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


