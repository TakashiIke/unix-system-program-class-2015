# Makefile
PROGRAM = bufcache
OBJS = dlist.o getblk.o parse.o main.o
CC = gcc
CFLAGS = -std=c99 -g 

$(PROGRAM) : $(OBJS)
	$(CC) -o $(PROGRAM) $^
parse.o : parse.c
	$(CC) $(CFLAGS) -c $<

dlist.o: dlist.c
	$(CC) $(CFLAGS) -c $<

getblk.o: getblk.c
	$(CC) $(CFLAGS) -c $<

main.o : main.c
	$(CC) $(CFLAGS) -c $<

clean : 
	$(RM)  *.o *~
