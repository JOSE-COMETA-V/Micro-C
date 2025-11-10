#include <stdio.h>

void suma(int m1[3][3], int m2[3][3], int resultado[3][3]);
void multiplicacion(int m1[3][3], int m2[3][3], int resultado[3][3]);

int main(){

    int m1[3][3], m2[3][3], resultado[3][3];

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf("Ingrese el numero en la posicion [%i][%i] para la matriz 1\n", i, j);
            scanf("%i", &m1[i][j]);
        }
        
    }
    
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf("Ingrese el numero en la posicion [%i][%i] para la matriz 2\n", i, j);
            scanf("%i", &m2[i][j]);
        }
        
    }

    suma(m1, m2, resultado);

    printf("Matriz de suma\n");
    printf("\n");

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf("%i ", resultado[i][j]);
        }
        printf("\n");
    }

    multiplicacion(m1, m2, resultado);


    printf("Matriz de multiplicacion\n");
    printf("\n");

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf("%i ", resultado[i][j]);
        }
        printf("\n");
    }

    return 0;
}

void suma(int m1[3][3], int m2[3][3], int resultado[3][3]){
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            resultado[i][j]=m1[i][j]+m2[i][j];
        }   
    }
    
}

void multiplicacion(int m1[3][3], int m2[3][3], int resultado[3][3]){
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            int r=0;
            for (int k = 0; k < 3; k++)
            {
                r += m1[i][k]*m2[k][j];
            }
            resultado[i][j]=r;
        }   
    }
}