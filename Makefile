COMPILER=gcc
CFLAGS=-Wall -g


all: mymalloctest

mymalloc.o: mymalloc.c mymalloc.h
	$(COMPILER) $(CFLAGS) -c mymalloc.c

mymalloctest.o: mymalloc.o mymalloctest.c
	$(COMPILER) $(CFLAGS) -c mymalloctest.c

mymalloctest: mymalloc.o mymalloctest.o
	$(COMPILER) $(CFLAGS) -o mymalloctest mymalloctest.c mymalloc.o

clean:
	\rm *.o *~