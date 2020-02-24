# Makefile for 'querier'
#
# David Kotz, April 2016, 2017
# updated by Xia Zhou, July 2016, 2018, January 2019
# updated by Temi Prioleau, January 2020
# utilized by Maria Roodnitsky, February 2020


OBJS = ../libcs50/hashtable.o ../libcs50/memory.o ../libcs50/webpage.o ../libcs50/counters.o ../libcs50/jhash.o ../libcs50/set.o ../libcs50/file.o ../common/word.o ../common/index.o ../common/pagedir.o ../libcs50/bag.o

TESTPROG = testing.sh

LIBS = -lm

# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../libcs50 -I../common
CC = gcc
MAKE = make

.PHONY = all clean 

all:
	$(MAKE) querier
	$(MAKE) input_tester
  
querier: querier.c
	$(CC) $(CFLAGS) $(OBJS) -lm -o querier querier.c 
  
input_tester: input_tester.c
	$(CC) $(CFLAGS) $(OBJS) -lm -o input_tester input_tester.c 


hashtable.o: ../libcs50/set.h
webpage.o: ../libcs50/file.h

test: $(TESTPROG)
	./$(TESTPROG)


clean:
	rm -f *~ *.o *.dSYM
	rm -f core
	rm -f vgcore.*
	rm -f querier
	rm -f input_tester