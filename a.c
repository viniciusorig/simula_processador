#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//#include "../includes/lib.h"
  
#define mem 0 
#define ope 1
#define sys 2

int reg[] = {0, 0, 0};

void
load(char *memory, int *ra)
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

    while(!feof(file))
    {
        c = getc(file);
 
        if(c == '\n')
        {
            i++;
            j = 0;
            matrix = (char **)realloc(matrix,(sizeof(char*) * (i+1)));
            matrix[i] = (char *)malloc(sizeof(char));
            continue;
        }
        matrix[i][j] = c;
        j++;
        *matrix = (char *)realloc(*matrix, (sizeof(char) * (j+1)));
    }
    *tam = i;
    return matrix;
}

//separo a intrucoes em pedacos

char **
stripper(char *instrucao, int *tam)
{
    char **matrix = (char **)malloc(sizeof(char *));
    *matrix = (char *)malloc(sizeof(char));
    char c = 0;
    int i = 0, j = 0, k = 0;

    while(c != ';')
    {
        c = instrucao[k];
    
        if(c == 'R' || c == ';') 
        {
            k++;
            continue;
        }
        if(c == ' ')
        {
            i++;
            j = 0;
            matrix = (char **)realloc(matrix,(sizeof(char*) * (i+1)));
            matrix[i] = (char *)malloc(sizeof(char));
            memset(matrix[i], 0, 1);
            k++;
            continue;
        }

        k++;
        matrix[i][j] = c;
        j++;
        *matrix = (char *)realloc(*matrix, (sizeof(char) * (j+1)));
        matrix[i][j] = 0;
    }
    *tam = i;
    return matrix;
}

void
instruction(char **matrix, int offset)
{
    int tam;
    char **step = stripper(matrix[offset], &tam);

    for(int i = 0; i <= tam; i++)
    {
       printf("%s\n", step[i]); 
    }

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
        load(matrix[atoi(step[2])], &reg[atoi(step[1])]);
        printf("carregou para o registrador %d da memoria %d\n\n",atoi(step[1]), atoi(step[2]));
        printf("%d\n", reg[atoi(step[1])]);
    }
    if(strncmp(step[0], "STORE", 5) == 0)
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
    
    return;
}

void
uc(FILE *memoria)
{
    int tam;
    char **instrucoes = memory(memoria, &tam);
    printf("memoria.txt\n\n");    

    for(int i = 0; i < tam; i++)
    {
        printf("%s", instrucoes[i]);
    }

    for(int i = 0; i <= tam; i++)
    {
        instruction(instrucoes, i);
    }
}

void
start(FILE *inetern)
{
    uc(inetern);
    //printf("foi ate o final?");
}

int
main()
{
    FILE *in = fopen("./arquivo_in/entrada.txt", "r");
    start(in);
   // printf("%d", tam_b);
    return 0;
}