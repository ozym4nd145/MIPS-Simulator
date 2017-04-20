
CC = gcc
CFLAGS = -g -Wall 

all:  sim

sim:  main.o cache.o lru.o
	$(CC) -o sim main.o cache.o lru.o -lm

main.o:  main.c cache.h
	$(CC) $(CFLAGS) -c main.c

cache.o:  cache.c cache.h
	$(CC) $(CFLAGS) -c cache.c

lru.o:  lru.c lru.h
	$(CC) $(CFLAGS) -c lru.c

clean: 
	rm -rf ./*.o
