#ifndef HASH_HEADER
#define HASH_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SYMBOL_IDENTIFIER 	1
#define SYMBOL_LIT_INTEGER	2
#define SYMBOL_LIT_FLOAT 	3
#define SYMBOL_LIT_CHAR 	4
#define SYMBOL_LIT_STRING 	5
#define SYMBOL_VARIABLE 	6
#define SYMBOL_FUNCTION 	7
#define SYMBOL_VECTOR    	8

#define DATATYPE_INT        9
#define DATATYPE_FLOAT      10
#define DATATYPE_CHAR       11
#define DATATYPE_STRING     12

#define HASH_SIZE 997

typedef struct hash_node{
    int datatype;
    int type;
    int params;
    char *text;
    struct hash_node *next;
} HASH_NODE;

void hashInit(void);
int hashAddress(char *text);
HASH_NODE *hashFind(char *text);
HASH_NODE *hashInsert(char *text, int type);
void hashPrint(void);
int hash_check_undeclared(void);

#endif
