# Makefile for 'indexer'
#
# David Kotz, April 2016, 2017
# updated by Xia Zhou, July 2016, 2018, January 2019
# updated by Temi Prioleau, January 2020
# utilized by Maria Roodnitsky, February 2020


OBJS1 = ../libcs50/hashtable.o ../libcs50/memory.o ../libcs50/webpage.o ../libcs50/counters.o ../libcs50/jhash.o ../libcs50/set.o ../libcs50/file.o ../common/word.o ../common/index.o ../common/pagedir.o ../libcs50/bag.o

LIBS = -lm

# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../libcs50 -I../common
CC = gcc
MAKE = make

.PHONY = all clean 

all:
	$(MAKE) querier
#	$(MAKE) indextest

querier: querier.c
	$(CC) $(CFLAGS) $(OBJS1) -lm -o querier querier.c 

#indextest: indextest.c
#	$(CC) $(CFLAGS) $(OBJS1) -lm -o indextest indextest.c 


hashtable.o: ../libcs50/set.h
webpage.o: ../libcs50/file.h

#test: $(TESTPROG)
#	./$(TESTPROG)


clean:
	rm -f *~ *.o *.dSYM
	rm -f indexer
	rm -f core
	rm -f vgcore.*
	rm -f indextest
	rm -f *.txt