#
# Makefile for AESStringCrypt test script (Linux)
# Copyright (C) 2007, 2008, 2009, 2012, 2015
# Paul E. Jones <paulej@packetizer.com>
#
# This software is licensed as "freeware."  Permission to distribute
# this software in source and binary forms is hereby granted without a
# fee.  THIS SOFTWARE IS PROVIDED 'AS IS' AND WITHOUT ANY EXPRESSED OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
# THE AUTHOR SHALL NOT BE HELD LIABLE FOR ANY DAMAGES RESULTING FROM THE
# USE OF THIS SOFTWARE, EITHER DIRECTLY OR INDIRECTLY, INCLUDING, BUT
# NOT LIMITED TO, LOSS OF DATA OR DATA BEING RENDERED INACCURATE.
#

CC=gcc

#The below line is for debugging
#CFLAGS=-I. -ggdb -Wall
CFLAGS=-Wall

LIBS=

#Uncomment the line below to compile on Mac
#LIBS=-liconv

sctest: sctest.o AESStringCrypt.o aes.o sha256.o
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $*.c

clean:
	rm -f *.o sctest
