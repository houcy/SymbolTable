#
# Mike Hoyt
# CS 442: Symbol Table
#
#####################################################################
#
# Type "make" to compile your code
# 
# Type "make clean" to remove the executable (and any object files)
#
#####################################################################
CC=gcc
CFLAGS=-Wall -g -O0
LDFLAGS=-lm

#
# List all of the binary programs you want to build here
# Separate each program with a single space
#
all: launcher

#
# Project main program
#

launcher: semantics.c main.c SymTab.c SymTab.h IOManager/IOMngr.c arithYacc.y semantics.h 
	yacc -d arithYacc.y
	lex arithLex.l 
	$(CC) -o launcher $(CFLAGS) y.tab.c lex.yy.c semantics.c SymTab.c IOManager/IOMngr.c main.c

#
# Cleanup the files that we have created
#
clean:
	$(RM) launcher
	$(RM) -rf *.dSYM *.o
