#
# Makefile that builds btest and other helper programs for the CS:APP data lab
# 
CC = gcc
CFLAGS = -O2 -Wall -m32
LIBFLAGS=-pthread

all: mm-mt mm

mm: mm.c mm.h 
	$(CC) $(CFLAGS) -o mm mm.c 

mm-mt: mm-mt.c mm-mt.h 
	$(CC) $(CFLAGS) $(LIBFLAGS) -o mm-mt mm-mt.c 

clean:
	rm -f *.o mm mm-mt
