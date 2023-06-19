#include <stdio.h>

#include "../includes/lib.h"

int
main(int argc, char **argv)
{
    FILE *entrada ;
    if((entrada = fopen("./arquivo_in/entrada.txt", "r")) == NULL)
    {
        fprintf(stderr, "erro main arquivo NULL");
        return 1;
    }
    start(entrada);
    return 0;
}
