#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int busqueda(int arreglo[], int longitud, int dato);
void organizar(int arreglo[], int longitud);

int main(){

    int longitud, dato;

    printf("Ingrese la longitud del arreglo:\n");
    scanf("%i", &longitud);
    int arreglo[longitud];

    srand(time(NULL));

    for (int i = 0; i < longitud; i++)
    {
        arreglo[i] = rand() % 31;
    }
    
    organizar(arreglo, longitud);
    for (int i = 0; i < longitud; i++)
    {
        printf("%i ", arreglo[i]);
    }
    printf("\nIngrese un numero de la lista\n");
    scanf("%i", &dato);

    printf("%i", busqueda(arreglo, longitud, dato)+1);

    return 0;
}

void organizar(int arreglo[], int longitud){

    int aux, conteo;
    //Metodo burbuja
    for (int i = 0; i < longitud-1; i++)
    {
        conteo = 0;
        for (int j = 0; j < longitud-i-1; j++)
        {
            if (arreglo[j] > arreglo[j+1])
            {
                ++conteo;
                aux = arreglo[j];
                arreglo[j] = arreglo[j+1];
                arreglo[j+1] = aux;
            }
        }
        if (conteo == 0)
        {
            break;
        }
    }
}

int busqueda(int arreglo[], int longitud, int dato){
    //Busqueda binaria
    int inf = 0, sup = longitud, mitad;
    char cond = 'f';
     while (inf <= sup)
    {
        mitad = (sup + inf) / 2;
        if (dato == arreglo[mitad])
        {
            cond = 'v';
            break;
        }
        if (dato > arreglo[mitad])
        {
            inf = mitad + 1;
        }
        if (dato < arreglo[mitad])
        {
            sup = mitad -1;
        }
    }
    
    return (cond == 'v') ? mitad : -1;
}