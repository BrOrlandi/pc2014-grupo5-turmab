#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NPONTOS 100000000 // total de pontos que serao gerados
#define LIMIT 10000000

const unsigned int A = 1664525, C = 1013904223;
int seed;

double next() {
    int ret = seed*A+C;
    seed = ret;
    return ((double) (ret%LIMIT)/LIMIT);
}

//Recebe quantos pontos serao gerados
int calcula(int pontos) {
    int pDentro,i; //pDentro = total de pontos que cairem dentro do circulo unitario
    double coodX, coodY;
    pDentro = 0;
    seed = time(NULL); //inicia seed para numeros aleatorios
    for(i=0;i<pontos;i++){
        coodX = next();
        coodY = next();
        //Para cada ponto verifica se esta dentro do circulo
        if(((coodX*coodX)+(coodY*coodY)) <= 1){
            pDentro++;
        }
    }
    return pDentro;
}

int main() {
    int pDentro;
    float pi;
    pDentro = calcula(NPONTOS);
    pi = 4*((double)pDentro/(double)NPONTOS);//calcula valor de pi
    printf("Valor de pi: %f\n", pi);//%f padrao imprime 6 casas decimais
    return 0;
}
