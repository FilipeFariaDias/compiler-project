#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hash.h"

HASH_NODE*Table[HASH_SIZE];

void hashInit(void){
    int i;

    for(i = 0; i < HASH_SIZE; i++){
        Table[i] = 0;
    }
}

int hashAddress(char *text){
    int address = 1;
    int i;

    for(i = 0; i < strlen(text); ++i){
        address = (address * text[i]) % HASH_SIZE + 1;
    }

    return address - 1;
}

HASH_NODE *hashInsert(char *text, int type){
    HASH_NODE *newnode;
    int address = hashAddress(text);

    if((newnode = hashFind(text)) != 0)
        return newnode;

    newnode = (HASH_NODE*) calloc(1, sizeof(HASH_NODE));
    newnode->type = type;
    newnode->text = (char*) calloc(strlen(text) + 1, sizeof(char));
    strcpy(newnode->text, text);

    newnode->next = Table[address];
    Table[address] = newnode;

    return newnode;
}

HASH_NODE *hashFind(char *text){
    int address = hashAddress(text);
    HASH_NODE* node;
    for(node = Table[address]; node != NULL; node = node->next){
        if(strcmp(text, node->text) == 0)
            return node;
    }
    return NULL;
}

void hashPrint(void){
    int i;
    HASH_NODE *node;

    for(i = 0; i < HASH_SIZE; ++i){
        for(node = Table[i]; node; node = node->next){
            printf("Table[%d] has %s, type %d, datatype %d\n", i, node->text, node->type, node->datatype);
        }
    }
}

int hash_check_undeclared() {
    int i;
    int undeclared = 0;
    HASH_NODE *node;
    for(i = 0; i < HASH_SIZE; i++)
        for(node = Table[i]; node; node = node->next)
            if(node->type == SYMBOL_IDENTIFIER) {
                fprintf(stderr, "SEMANTIC ERROR: identifier %s undeclared\n", node->text);
                ++undeclared;
            }

    return undeclared;
}
