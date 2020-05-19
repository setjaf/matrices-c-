//Autor: Set Jafet Renedo Ortega
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include "matriz.h"

using namespace std;

int main(int argc, char const *argv[])
{
    int numMatrices = 4;
    int columnas, filas;
    srand(time(NULL));

    printf("¿Cuántas matrices quieres crear? ");
    scanf("%d",&numMatrices);

    Matriz **matrices = (Matriz **) malloc(sizeof(Matriz *)*numMatrices);

    /*matrices[0] = new Matriz(2,3);
    matrices[1] = new Matriz(3,4);
    matrices[2] = new Matriz(4,2);
    matrices[3] = new Matriz(2,1);*/

    for (int i = 0; i < numMatrices; i++)
    {
        printf("---------------------------\n"); 
        printf("Filas:");
        scanf("%d",&filas);
        printf("Columnas:");
        scanf("%d",&columnas);
        matrices[i] = new Matriz(filas, columnas);
    }
    

    printf("\n***************  Llenar matrices  ***************\n");
    for (int k = 0; k < numMatrices; k++)
    {
        for (int i = 0; i < matrices[k]->filas; i++)
        {
            for (int j = 0; j < matrices[k]->columnas; j++)
            {
                matrices[k]->matriz[i][j] = rand() % 5 + 1;
                printf("%.2f\t",matrices[k]->matriz[i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }
    printf("\n***************  ***************  ***************\n");
    Matriz *nueva = Matriz::multiplicacionDinamica(matrices,numMatrices);

    if (nueva != NULL)
    {
        for (int i = 0; i < nueva->filas; i++)
        {
            for (int j = 0; j < nueva->columnas; j++)
            {
                printf("%.2f\t", nueva->matriz[i][j]);
            }
            printf("\n");
        }
    }else{
        printf("Las matrices no se pueden multiplicar\n");
    }  

    return 0;
}
