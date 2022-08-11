# makefile for user Buffer Mon

CC=gcc
CFLAGS=-Wall

clean:
	rm -rf *.o

buff: buffer_mon.o test_mon.o
	$(CC) $(CFLAGS) -o buff buffer_mon.o test_mon.o -lpthread

test_mon.o: test_mon.c
	$(CC) $(CFLAGS) -c test_mon.c

buffer_mon.o: buffer_mon.c buffer_mon.h
	$(CC) $(CFLAGS) -c buffer_mon.c

