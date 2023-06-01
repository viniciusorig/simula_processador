OUT = bin/processador.out
FILES =   
OBJ = $(PWD)/lib
CC = gcc

all:
	$(CC) -Wall lib/lib.c -c 
	$(CC) -Wall src/main.c lib.o -o $(OUT)
#	$(CC) -Wall ./bin/main.o ./bin/lib.o -o a.out	

test:
	$(CC) -Wall a.c -o test.out

clean:
	rm -f test.out ./bin/processador.out 