# Makefile for 'common'
#
# David Kotz, April 2016, 2017
# updated by Xia Zhou, July 2016, 2018, January 2019
# updated by Temi Prioleau, January 2020
# used by Maria Roodnitsky, February 2020

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../libcs50
LIB = common.a

OBJS = pagedir.o ../libcs50/webpage.o ../libcs50/memory.o
LIBS = -lm

# uncomment the following to turn on verbose memory logging
# TESTING=-DMEMTEST



MAKE = make

# Build the library by archiving object files
$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

pagedir.o: ../libcs50/webpage.h pagedir.h ../libcs50/memory.h


.PHONY: clean

# test: $(PROG) test_names
#	./$(PROG) < test_names

clean:
	rm -f *~ *.o *.dSYM
	rm -f $(PROG)
	rm -f stocks