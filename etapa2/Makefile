##Alunos: Filipe Faria Dias - 271423
##       Germano Girondi - 288701

#
# UFRGS - Compiladores B - Marcelo Johann - 2022/1 - Etapa 2
#
# Makefile for single compiler call
# All source files must be included from code embedded in scanner.l
# In our case, you probably need #include "hash.c" at the beginning
# and #include "main.c" in the last part of the scanner.l
#

etapa2: y.tab.c lex.yy.c 
	gcc -o etapa2 lex.yy.c
y.tab.c: parser.y
	yacc -d parser.y
lex.yy.c: scanner.l 
	lex scanner.l

clean:
	rm -f *.o etapa2 lex.yy.c lex.yy.h y.tab.c y.tab.h y.output
