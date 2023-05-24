OUT = ./bin/processador.out
FILES = ./lib/lib.c ./src/main.c
OBJ = ./lib/lib.o ./src/main.o
CC = gcc

all:
	$CC $OBJ -o $OUT	

OBJ:
	$CC $FILES -o $OBJ	