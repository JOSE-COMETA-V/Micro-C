#include <stdio.h>

float Area(float *base, float *altura, float *area);

int main(){
    float base, altura, area;

    Area(&base, &altura, &area);
    printf("El resultado es: %.2f", area);
    return 0;
}

float Area(float *base, float *altura, float *area){
    printf("Ingrese la base\n");
    scanf("%f", &*base);
    printf("Ingrese la altura\n");
    scanf("%f", &*altura);
    *area = *base *(*altura)/2;
    return *area;
}