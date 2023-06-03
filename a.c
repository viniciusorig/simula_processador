#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//#include "../includes/lib.h"
  
#define mem 0 
#define ope 1
#define sys 2

int reg[] = {0, 0, 0};

struct Uc
{
    int res_ula;
    int pc;
    int ir;
    int signal;
};


void
load(char *memory, int *ra)
{
    *ra = atoi(memory);
}

void 
store(char *memory, int *ra)
{
    int i  = 0;
    char *ha = malloc(sizeof(char));
    for(; *ra < 1; i++)
    {
        *ra /= 10;
    }
    i++;
    ha = realloc(ha, sizeof(char)*(i+1));
    memory = realloc(memory, sizeof(char)*(i+1));
    snprintf(ha, (i+1), "%d", *ra);
    strncpy(memory, ha, i+1);
}

void
move(int *ra, int *rb)
{
    *ra = *rb;
}

void 
sub(int *ra, int rb, int rc, int *ula)
{
    *ra = rb + rb;
    *ula = *ra;
}

void
add(int *ra, int rb, int rc, int *ula)
{
    *ra = rb+rc;
    *ula = *ra;

}

void 
or(int * ra, int rb, int rc, int *ula )
{
    *ra = rb | rc;
    *ula = *ra;
}

void
and(int *ra, int rb, int rc, int *ula)
{
    *ra = rb & rc;
    *ula = *ra;
}

//system suspende
int 
halt()
{
    fprintf(stdout, "saida\n");
    return -1;
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
    fclose(file);

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
instruction(char **matrix, int offset, size_t total, struct Uc *uc)
{
    int tam;
    char **step = stripper(matrix[offset], &tam);

    if(strncmp(step[0], "ADD", 3) == 0)
    {
        add(&reg[atoi(step[1])], reg[atoi(step[2])], reg[atoi(step[3])], &uc->res_ula);
        
        uc->signal = 1;
    }
    if(strncmp(step[0], "SUB", 3) == 0)
    {
        sub(&reg[atoi(step[1])], reg[atoi(step[2])], reg[atoi(step[3])], &uc->res_ula);
        uc->signal = 1;
    }
    if(strncmp(step[0], "AND", 3) == 0)
    {
        and(&reg[atoi(step[1])], reg[atoi(step[2])], reg[atoi(step[3])], &uc->res_ula);
        uc->signal = 1;
    }
    if(strncmp(step[0], "OR", 2) == 0)
    {
        or(&reg[atoi(step[1])], reg[atoi(step[2])], reg[atoi(step[3])], &uc->res_ula);
        uc->signal = 1;
    }

    if(strncmp(step[0], "LOAD", 4) == 0)
    {
        load(matrix[atoi(step[2])], &reg[atoi(step[1])]);
        uc->signal = 1;
    }
    if(strncmp(step[0], "STORE", 3) == 0)
    {
        store(matrix[atoi(step[1])], &reg[atoi(step[2])]);
        uc->signal = 1;
    }
    if(strncmp(step[0], "MOVE", 4) == 0)
    {
        move(&reg[atoi(step[1])], &reg[atoi(step[2])]);
        uc->signal = 1;
    }
    
    if(strncmp(step[0], "HALT", 4) == 0)
    {
        halt();
        uc->signal = -1;
    }
    if(strncmp(step[0], "NOP", 3) == 0)
    {
        nop();
        uc->signal = 1;
    }
}

void
finish(char **memory, struct Uc *uc, int tam)
{
    FILE *memory_out, *controle, *ula;
    if(((memory_out = fopen("./arquivos_out/memory_dump.txt", "w")) == NULL ) || 
        ((ula = fopen("./arquivos_out/ula_dump.txt", "w")) == NULL)           || 
             ((controle = fopen("./arquivos_out/uc_dump.txt", "w")) == NULL))
    {
        fprintf(stderr, "nao foi possivel creiar/abrir os arquivos de saida");
        exit(EXIT_FAILURE);
    }
    
    for(int i = 0; i < (tam+1); i++)
    {
        fprintf(memory_out, "%s\n", memory[i]);
    }
    fclose(memory_out);

    fprintf(controle, "pc :: %d\nir :: %d", uc->pc, uc->ir);
    fclose(controle);

    fprintf(ula, "ultimo resultado da ula :: %d", uc->res_ula);
    fclose(ula);
}

void
UC(FILE *memoria)
{
    struct Uc *uc = malloc(sizeof(struct Uc)); 
    uc->pc = 1;
    int tam;
    char **instrucoes = memory(memoria, &tam);

    for(uc->ir = 0; uc->signal != -1; uc->pc++)
    {
        instruction(instrucoes, uc->ir, tam, uc);
        uc->ir = uc->pc;
    }
    finish(instrucoes, uc, tam);
}

void
start(FILE *inetern)
{
    UC(inetern);
}

int
main()
{
    FILE *in = fopen("./arquivo_in/entrada.txt", "r");
    start(in);
   // printf("%d", tam_b);
    return 0;
}