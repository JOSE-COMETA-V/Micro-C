#include <stdio.h>
#include <string.h>

void invertir(char cadena[]);

int main(){

    char cadena[10];

    printf("Ingrese una palabra\n");
    fgets(cadena, 10, stdin);
    strtok(cadena, "\n");
    invertir(cadena);
    printf("%s", cadena);
    return 0;
}

void invertir(char cadena[]){
    char temp, cad[10];
    strcpy(cad, cadena);
    for (int i = 0; i < 10; i++)
    {
        temp = cadena[i];
        cadena[i] = cad[9-i-1];
        cad[9-i-1] = temp;

    }
}