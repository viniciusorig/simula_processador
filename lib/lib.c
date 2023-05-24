#include <stdio.h>
#include <stdlib.h>


#include "../includes/lib.h"

char *OPCODES[12] = 
{
    "LOAD", "STORE" , "MOVE" , 
    "ADD" , "SUB"   , "AND"  ,
    "OR"  , "BRANCH", "BZERO",
    "BNEG", "NOP"   , "HALT"
};

//armazena tudo que esta dentro da memoria para dentro de uma matrix
char **
memory(FILE *file, int *tam)
{
    if(file == NULL)
    {
        fprintf(stderr, "erro ao ler o arquivo");
        exit(EXIT_FAILURE);
    }
    
    char **matrix = (char **)malloc(sizeof(char *));
    *matrix = (char *)malloc(sizeof(char));
    char c = 0;
    int i = 0, j = 0;

    while(feof(file))
    {
        c = fgetc(file);
 
        if(c == '\n')
        {
            i++;
            j = 0;
            matrix = (char **)realloc(matrix,(sizeof(char*) * i++));
            continue;
        }
        matrix[i][j] = c;
        j++;
        *matrix = (char *)realloc(*matrix, (sizeof(char) * j++));
    }

    return matrix;
}

//separo a intrucoes em pedacos
char **
stripper(char *instrucao)
{
    char **matrix = (char **)malloc(sizeof(char *));
    *matrix = (char *)malloc(sizeof(char));
    char c = 0;
    int i = 0, j = 0;

    while(instrucao != '\0')
    {
        c = *instrucao;
 
        if(c == ' ')
        {
            i++;
            j = 0;
            matrix = (char **)realloc(matrix,(sizeof(char*) * i++));
            continue;
        }
        matrix[i][j] = c;
        j++;
        *matrix = (char *)realloc(*matrix, (sizeof(char) * j++));
        instrucao++;
    }

}

void
uc(FILE *memoria, char r0, char r1, char r2, char r3)
{
    int tam;
    char **instrucoes = memory(memoria, tam);
}


void
instruction(char **matrix)
{
    char **step = stripper(*matrix);  
}