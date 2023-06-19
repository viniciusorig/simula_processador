#include <stdio.h>

#include "../includes/lib.h"

int
main(int argc, char **argv)
{
    FILE *entrada ;
    //abre o arquivo onde a memoria esta localizada e ja faz a vericacao se foi possivel abrir
    if((entrada = fopen("./arquivo_in/entrada.txt", "r")) == NULL) 
    {
        fprintf(stderr, "erro main arquivo NULL"); 
        return -1;
    }
    start(entrada);//chama o inio do trabalho do processador
    return 0;
}
