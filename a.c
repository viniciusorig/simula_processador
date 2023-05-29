#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    while(!feof(file))
    {
        c = fgetc(file);
 
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

int
main()
{
    FILE *in = fopen("./arquivo_in/entrada.txt", "r");
    int tam_a = 0, tam_b = 0;
    char **instrucao = memory(in, &tam_a);
    for(int i = 0; i <= tam_a; i++)
    {
        char **decode = stripper(instrucao[i], &tam_b);
        for(int j = 0; j <=  tam_b; j++)
        {
            setbuf(stdin, NULL);
            printf("%s\n", decode[j]); 
        }
        printf("\n\n");
    }
   // printf("%d", tam_b);
    return 0;
}