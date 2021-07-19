#
# Makefile that builds btest and other helper programs for the CS:APP data lab
# 
CC = gcc
CFLAGS = -O2 -Wall -m32
LIBFLAGS=-pthread

all: mm-t

mm: mm.c mm.h 
	$(CC) $(CFLAGS) -o mm mm.c 

mm-t: mm-t.c mm-t.h 
	$(CC) $(CFLAGS) $(LIBFLAGS) -o mm-t mm-t.c 

clean:
	rm -f *.o mm mm-t
