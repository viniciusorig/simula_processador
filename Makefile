OUT = bin/processador.out
FILES =   
OBJ = $(PWD)/lib/
CC = gcc

all:
	$(CC) -Wall lib/lib.c -c
	$(CC) -Wall src/main.c lib.o -o bin/33333.out
#	$(CC) -Wall ./bin/main.o ./bin/lib.o -o a.out	