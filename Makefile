# Makefile for iii (CS 40 Assignment 2)
# 
# Includes build rules for sudoku, unblackedges, my_useuarray2, and my_usebit2.
#
# This Makefile is more verbose than necessary.  In each assignment
# we will simplify the Makefile using more powerful syntax and implicit rules.
#
# New syntax rules added:  wildcards (%.o, %.c) and special variables:
# $@ - the target of the rule
# $< - the first dependency
# $^ - all dependencies
#
# Last updated: February 4, 2016


############## Variables ###############

CC = gcc # The compiler being used

# Updating include path to use Comp 40 .h files and CII interfaces
IFLAGS = -I. -I/comp/40/build/include -I/usr/sup/cii40/include/cii

# Compile flags
# Set debugging information, allow the c99 standard,
# max out warnings, and use the updated include path
CFLAGS = -g -std=c99 -Wall -Wextra -Werror -Wfatal-errors -pedantic $(IFLAGS)

# Linking flags
# Set debugging information and update linking path
# to include course binaries and CII implementations
LDFLAGS = -g -L/comp/40/build/lib -L/usr/sup/cii40/lib64

# Libraries needed for linking
# Both programs need cii40 (Hanson binaries) and *may* need -lm (math)
# Only brightness requires the binary for pnmrdr.
LDLIBS = -lpnmrdr -lcii40 -lm

# Collect all .h files in your directory.
# This way, you can never forget to add
# a local .h file in your dependencies.
INCLUDES = $(shell echo *.h)

############### Rules ###############

all: sudoku unblackedges my_useuarray2 my_usebit2


## Compile step (.c files -> .o files)

# To get *any* .o file, compile its .c file with the following rule.
%.o: %.c $(INCLUDES)
	$(CC) $(CFLAGS) -c $< -o $@


## Linking step (.o -> executable program)

sudoku: sudoku.o uarray2.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

unblackedges: unblackedges.o bit2.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

my_useuarray2: useuarray2.o uarray2.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

useuarray2.o: useuarray2.c uarray2.h
	$(CC) $(CFLAGS) -c useuarray2.c -o useuarray2.o

my_usebit2: usebit2.o bit2.o 
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

# bit2.o: bit2.c bit2.h
# 	$(CC) $(LDFLAGS) -c bit2.c -o bit2.o $(LDLIBS)

usebit2.o: usebit2.c bit2.h
	$(CC) $(CFLAGS) -c usebit2.c -o usebit2.o

clean:
	rm -f sudoku unblackedges my_useuarray2 my_usebit2 *.o

