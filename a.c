#include <stdio.h>

char *OPCODES[12] = 
{
    "LOAD", "STORE" , "MOVE" , 
    "ADD" , "SUB"   , "AND"  ,
    "OR"  , "BRANCH", "BZERO",
    "BNEG", "NOP"   , "HALT"
};

int
main()
{
    int a = 12;
    for(int i = 0; i < a; i++)
    {
        printf("%d\n", (int)OPCODES[i][0]%12);
    }

    return 0;
}