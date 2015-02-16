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
all: IODriver

#
# Project main program
#
IOMngr.o: IOMngr.c IOMngr.h
	$(CC) -o IOMngr.o $(CFLAGS) -c IOMngr.c

IODriver: IOMngr.o IODriver.c
	$(CC) -o IODriver IODriver.c IOMngr.o $(CFLAGS) $(LDFLAGS)

#
# Cleanup the files that we have created
#
clean:
	$(RM) IOMngr IODriver
	$(RM) -rf *.dSYM *.o
