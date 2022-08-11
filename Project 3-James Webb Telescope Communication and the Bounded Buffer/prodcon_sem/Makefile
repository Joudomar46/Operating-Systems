# makefile for Buffer
obj-y := buffer_sem.o

CC=gcc
CFLAGS=-Wall

clean:
	rm -rf *.o

buff: buffer_user_sem.o test_user_sem.o
	$(CC) $(CFLAGS) -o buff buffer_user_sem.o test_user_sem.o -lpthread

test_user_sem.o: test_user_sem.c
	$(CC) $(CFLAGS) -c test_user_sem.c

buffer_user_sem.o: buffer_user_sem.c buffer_sem.h
	$(CC) $(CFLAGS) -c buffer_user_sem.c

kernelTest:
	$(CC) $(CFLAGS) test_sem.c -o test -lpthread
