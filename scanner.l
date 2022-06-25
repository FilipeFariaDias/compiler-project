%{
    #include <stdio.h>
    #include "hash.c"
    #include "tokens.h"

    int lineNumber = 1;

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

%%


int getLineNumber(void){
    return lineNumber;
}


