#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../includes/lib.h"
  
#define mem 0 
#define ope 1
#define sys 2

int reg[] = {0, 0, 0};

void
load(char memory, int *ra)
{
    *ra = atoi(memory);
}

void 
store(char *memory, int *ra)
{
    int i  = 0, aux = atoi(memory);
    char *ha = 0;
    for(; aux < 1; i++)
    {
        aux /= 10;
    }

    memory = realloc(memory, sizeof(char)*i);
    snprintf(ha, i+1, "%d", *ra);
    strncpy(memory, ha, i+1);
}

void
move(int *ra, int *rb)
{
    *ra = *rb;
}

void 
sub(int *ra, int rb, int rc)
{
    *ra = rb + rb;
}

void
add(int *ra, int rb, int rc)
{
    *ra = rb+rc;
}

void 
or(int * ra, int rb, int rc)
{
    *ra = rb | rc;
}

void
and(int *ra, int rb, int rc)
{
    *ra = rb & rc;
}

//system suspende
void
halt()
{
    while(1);
}

//nao faz nada
void
nop()
{
    return;
}

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
 
        if(c == 'R') continue;
        
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
uc(FILE *memoria)
{
    int tam;
    char **instrucoes = memory(memoria, &tam);
    for(int i = 0; i < tam; i++)
    {
        instruction(instrucoes, i);
    }
}


void
instruction(char **matrix, int offset)
{
    char **step = stripper(matrix[offset]);
    if(strncmp(step[0], "ADD", 3) == 0)
    {
        add(&reg[atoi(step[1])], reg[atoi(step[2])], reg[atoi(step[3])]);
    }
    if(strncmp(step[0], "SUB", 3) == 0)
    {
        sub(&reg[atoi(step[1])], reg[atoi(step[2])], reg[atoi(step[3])]);
    }
    if(strncmp(step[0], "AND", 3) == 0)
    {
        and(&reg[atoi(step[1])], reg[atoi(step[2])], reg[atoi(step[3])]);
    }
    if(strncmp(step[0], "OR", 2) == 0)
    {
        or(&reg[atoi(step[1])], reg[atoi(step[2])], reg[atoi(step[3])]);
    }

    if(strncmp(step[0], "LOAD", 4) == 0)
    {
        load(matrix[atoi(step[1])], &reg[atoi(step[2])]);
    }
    if(strncmp(step[0], "ADD", 3) == 0)
    {
        store(matrix[atoi(step[1])], &reg[atoi(step[2])]);
    }
    if(strncmp(step[0], "MOVE", 4) == 0)
    {
        move(&reg[atoi(step[1])], &reg[atoi(step[2])]);
    }
    
    if(strncmp(step[0], "HALT", 4) == 0)
    {
        halt();
    }
    if(strncmp(step[0], "NOP", 3) == 0)
    {
        nop();
    }
    
    return 0
}