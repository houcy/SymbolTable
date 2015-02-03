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
all: driver

#
# Project main program
#
SymTab.o: SymTab.c SymTab.h
	$(CC) -o SymTab.o $(CFLAGS) -c SymTab.c

driver: SymTab.o driver.c
	$(CC) -o driver driver.c SymTab.o $(CFLAGS) $(LDFLAGS)

#
# Cleanup the files that we have created
#
clean:
	$(RM) SymTab driver
	$(RM) -rf *.dSYM *.o
