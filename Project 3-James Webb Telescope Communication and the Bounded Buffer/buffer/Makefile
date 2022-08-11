# makefile for Buffer
obj-y := buffer.o

CC=gcc
CFLAGS=-Wall

clean:
	rm -rf *.o

buff: buffer_user.o test_user.o
	$(CC) $(CFLAGS) -o buff buffer_user.o test_user.o

test_user.o: test_user.c
	$(CC) $(CFLAGS) -c test_user.c

buffer_user.o: buffer_user.c buffer.h
	$(CC) $(CFLAGS) -c buffer_user.c

kernelTest:
	$(CC) $(CFLAGS) test.c -o test
