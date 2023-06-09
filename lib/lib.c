#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../includes/lib.h"

//sinais de encerrameto ou continuacao da uc

#define CONTINUE 1  
#define HALT -1 

//registradores

int reg[] = {0, 0, 0};

//strutura uc

struct Uc
{
    int res_ula;
    int pc;
    int ir;
    int signal;
};

//funcoes da ula

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
    return HALT;
}

//nao faz nada
void
nop()
{
    return;
}

void
branch(struct Uc *uc, int pocisao)
{
    uc->pc = pocisao;
    return;
}

void
Bzero(struct Uc *uc, int pocisao)
{
    if(uc->res_ula == 0)
    {
        uc->pc = pocisao;
        return;
    }
    return;
}

void
bneg(struct Uc *uc, int pocisao)
{
    if(uc->res_ula < 0)
    {
        uc->pc = pocisao;
        return;
    }
    return;
}

//leitura da memoria e passando para uma matriz de caracteres

char **
memory(FILE *file, int *tam)
{
    if(file == NULL) //verificacao se o arquiivo existe
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

//"estripando" a intrucoes

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

//decode de cadda intrucaao

void
instruction(char **matrix, int offset, size_t total, struct Uc *uc)
{
    int tam;
    char **step = stripper(matrix[offset], &tam); //recebendo a funcao separada em parametros
    
    /*parte vergonhosa, se possivel ignorar mas caso nao esta eh a parte onde verifico qual eh cada funcao*/
    
    if(strncmp(step[0], "ADD", 3) == 0)
    {
        add(&reg[atoi(step[1])], reg[atoi(step[2])], reg[atoi(step[3])], &uc->res_ula);
        uc->signal = CONTINUE;
        return;
    }

    if(strncmp(step[0], "SUB", 3) == 0)
    {
        sub(&reg[atoi(step[1])], reg[atoi(step[2])], reg[atoi(step[3])], &uc->res_ula);
        uc->signal = CONTINUE;
        return;
    }

    if(strncmp(step[0], "AND", 3) == 0)
    {
        and(&reg[atoi(step[1])], reg[atoi(step[2])], reg[atoi(step[3])], &uc->res_ula);
        uc->signal = CONTINUE;
    }

    if(strncmp(step[0], "OR", 2) == 0)
    {
        or(&reg[atoi(step[1])], reg[atoi(step[2])], reg[atoi(step[3])], &uc->res_ula);
        uc->signal = CONTINUE;
        return;
    }

    if(strncmp(step[0], "LOAD", 4) == 0)
    {
        load(matrix[atoi(step[2])], &reg[atoi(step[1])]);
        uc->signal = CONTINUE;
        return;
    }

    if(strncmp(step[0], "STORE", 3) == 0)
    {
        store(matrix[atoi(step[1])], &reg[atoi(step[2])]);
        uc->signal = CONTINUE;
        return;
    }
    
    if(strncmp(step[0], "MOVE", 4) == 0)
    {
        move(&reg[atoi(step[1])], &reg[atoi(step[2])]);
        uc->signal = CONTINUE;
        return;
    }
    
    if(strncmp(step[0], "HALT", 4) == 0)
    {
        halt();
        uc->signal = HALT;
        return;
    }
    
    if(strncmp(step[0], "NOP", 3) == 0)
    {
        nop();
        uc->signal = CONTINUE;
        return;
    }
    
    if(strncmp(step[0], "BNEG", 4) == 0)
    {
        bneg(uc, atoi(step[1]));
        uc->signal = CONTINUE;
        return;
    }
    
    if(strncmp(step[0], "BZERO", 3) == 0)
    {
        Bzero(uc, atoi(step[1]));
        uc->signal = CONTINUE;
        return;
    }
    
    if(strncmp(step[0], "BRANCH", 1) == 0)
    {
        branch(uc, atoi(step[1]));
        uc->signal = CONTINUE;
        return;
    }
    /*parte vefonhosa finalizada*/
}

/*dump para os arquivos*/

void
finish(char **memory, struct Uc *uc, int tam)
{
    FILE *memory_out, *controle, *ula;

    //verificacao sobre a possibilidade de abertura dos arquivos de saida

    if(((memory_out = fopen("./arquivos_out/memory_dump.txt", "w")) == NULL ) || 
        ((ula = fopen("./arquivos_out/registrador_dump.txt", "a")) == NULL)           ||
             ((controle = fopen("./arquivos_out/uc_dump.txt", "a")) == NULL))
    {
        fprintf(stderr, "nao foi possivel creiar/abrir os arquivos de saida");
        exit(EXIT_FAILURE);
    }
    
    //faco o dump da memoria

    for(int i = 0; i < (tam+1); i++)
    {
        fprintf(memory_out, "%s\n", memory[i]);
    }

    //fecho o arquivo da memoria

    fclose(memory_out);

    //dump do uc e da ula  

    if(uc->pc < 1)
    {
        fprintf(controle, "pc :: %d\tir :: %s\n\n", uc->pc, memory[0]);
        fprintf(ula, "pc :: %d\tir :: %s\n", uc->pc, memory[0]);
    }
    else
    {   
        fprintf(controle, "pc :: %d\tir :: %s\n\n", uc->pc, memory[uc->pc-1]);
        fprintf(ula, "pc :: %d\tir :: %s\n", uc->pc, memory[uc->pc-1]);
    }

    //arquivo fechado da uc

    fclose(controle);

    //dump dos registradores

    for(int i = 0; i < 3; i++)
    {
        fprintf(ula, "registrador %d :: %d\n", i, reg[i]);
    }
    fprintf(ula, "\n\n");
    
    //arquivo fechado dda ula

    fclose(ula);
    return;
}

//unidade de controle

void
UC(FILE *memoria)
{
    struct Uc *uc = malloc(sizeof(struct Uc)); //instaceio a struct que conten os valores
    uc->pc = 1;
    int tam;
    char **instrucoes = memory(memoria, &tam); //recebo a matriz da memoria

    for(uc->ir = 0; uc->signal != HALT; uc->pc++) //vou realisando as operacoes ate encontrar o sinal que finaliza que indica o final das operacoes
    {
        instruction(instrucoes, uc->ir, tam, uc); //gerencia cada instrucao
        uc->ir = uc->pc;//atualizo ir
        finish(instrucoes, uc, tam);//dump para os arquivo
    }
    return;
}

//funcao que inicializa todos os outros

void
start(FILE *inetern)
{
    system("rm -rf arquivos_out/*"); //se extirem outros arquivos de saida remove todos eles
    UC(inetern); //inicia a unidade de controle
}
