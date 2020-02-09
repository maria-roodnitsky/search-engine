# Makefile for 'crawler'
#
# David Kotz, April 2016, 2017
# updated by Xia Zhou, July 2016, 2018, January 2019
# updated by Temi Prioleau, January 2020
# utilized by Maria Roodnitsky, February 2020

PROG = crawler
PROG2 = testing.sh
OBJS = crawler.o ../libcs50/hashtable.o ../libcs50/memory.o ../libcs50/webpage.o ../libcs50/bag.o ../libcs50/jhash.o ../libcs50/set.o ../libcs50/file.o ../common/pagedir.o
LIBS = -lm

# uncomment the following to turn on verbose memory logging
TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../libcs50 -I../common
CC = gcc
MAKE = make

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(PROG)

crawler.o: ../libcs50/hashtable.h ../libcs50/jhash.h ../libcs50/set.h ../libcs50/memory.h ../libcs50/bag.h ../libcs50/webpage.h ../libcs50/file.h ../libcs50/jhash.h ../common/pagedir.h
hashtable.o: ../libcs50/set.h
webpage.o: ../libcs50/file.h

.PHONY: clean

test: $(PROG2)
	./$(PROG2)


clean:
	rm -f *~ *.o *.dSYM
	rm -f $(PROG)