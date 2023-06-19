OUT = bin/processador.out
FILES =   
OBJ = $(PWD)/lib
CC = gcc

all:
	$(CC) -Wall -c lib/lib.c -o bin/lib.o
	$(CC) -Wall src/main.c bin/lib.o -o $(OUT)

test:
	$(CC) -Wall a.c -o test.out

clean:
	rm -f test.out ./bin/processador.out lib.o test.out a arquivos_out/*