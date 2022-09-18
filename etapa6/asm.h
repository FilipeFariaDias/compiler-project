#ifndef ASM_H
#define ASM_H

#include "tac.h"
#include "ast.h"

//Essas funções tão presentes em trabalhos antigos, mas não sei se procedem. Conferir
void johannNumberConversion(char* johann, char* out);

void addTemporaries(FILE* out);

void addImmediates(FILE* out);

void addData(FILE* out, AST* node);

void asmGenerate(TAC* newtac, AST* ast);


#endif
