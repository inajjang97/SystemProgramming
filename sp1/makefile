CC=gcc
CFLAGS=-g -Wall
OBJS=20161601.o
TARGET=20161601.out

$(TARGET) : $(OBJS)
		$(CC) -o $@ $(OBJS)

20161601.o : 20161601.c 20161601.h
	$(CC) -c 20161601.c -Wall

clean : 
	rm 20161601.o
	rm 20161601.out
