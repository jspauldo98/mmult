###############################################################################
# Makefile 
# Jared Spaulding
# COSC 3750, SP 2020
# Homework 11
# May 15th, 2020
# 
# Makefile to complile mmult
###############################################################################

CC=gcc
CFLAGS=-ggdb -Wall
RM=/bin/rm -f

.PHONY: tidy clean

mmult: mmult.c matrix.c
	$(CC) $(CFLAGS) mmult.c matrix.c -o mmult

tidy:
	$(RM) *.o

clean: tidy
	$(RM) mmult
	$(RM) core.*