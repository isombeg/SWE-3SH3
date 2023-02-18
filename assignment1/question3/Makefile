CC = gcc
LC = gcc

CFLAGS = -Wall -pipe -g
OFLAGS = -c -I/usr/include
LFLAGS = $(CFLAGS) -L/usr/lib/

SOURCES = $(wildcard *.c)
OBJS = $(SOURCES:.c=.o)

INCLUDE = question3.c context.c student.c studentQueue.c ta.c
LIBS = -lm -lpthread

all: question3

project: $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o question3 $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o project

rebuild: clean all