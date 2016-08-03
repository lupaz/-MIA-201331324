#include <stdio.h>
#include <stdlib.h>


typedef struct{
    int numeroToken;
    struct tokens *siguiente;
}tokens;



int main()
{
    printf("Hola este es el primer Commit!\n");


    printf("antes:\n\n");

    tokens *tok1= malloc(sizeof(tokens));
    tok1->numeroToken=1;

    printf(" el token es %i \n",tok1->numeroToken);
    printf("llego");
    return 0;
}
