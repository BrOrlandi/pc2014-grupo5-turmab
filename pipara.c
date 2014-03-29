#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define NTHREADS 4
#define NPONTOS 100000000
#define LIMIT 10000000

int pDentro[NTHREADS];

const unsigned int A = 1664525, C = 1013904223;

void *calcula (void *parametro){
    int i, pontos;
    float coodX, coodY;
    long threadID;

    int x = rand();

    threadID = (long)parametro;//recuperou id da thread que esta sendo executada
    pontos  = NPONTOS/NTHREADS;//total de pontos que serao calculados por essa thread
    pDentro[threadID] = 0;
    for(i=0;i<pontos;i++){
        x = x*A+C;
        coodX = ((double) (x%LIMIT)/LIMIT);
        x = x*A+C;
        coodY = ((double) (x%LIMIT)/LIMIT);
        //Para cada ponto verifica se esta dentro do circulo
        if(((coodX*coodX)+(coodY*coodY)) <= 1){
            pDentro[threadID]++;
        }
    }
    pthread_exit(0);
}

int main (){
    int pontos = 0;
    long i;
    float pi;
    pthread_t threadID[NTHREADS];

    srand(time(NULL)); //inicia seed para numeros aleatorios

    for (i = 0; i< NTHREADS ; i++){
        pthread_create (&threadID[i], NULL, calcula, (void*)i);//Cria cada thread com a funcao calcula e passando o respectivo ID
    }

    for (i = 0; i< NTHREADS ; i++){
        pthread_join (threadID[i], NULL);//Espera cada thread terminar de executar
    }

    for(i = 0; i< NTHREADS;i++){
        pontos = pontos+pDentro[i];//Soma todos os pontos que cairam dentro do circulo de cada thread
    }

    pi = 4*((double)pontos/(double)NPONTOS);//calcula valor de pi

    printf("Valor de pi: %f\n", pi);//%f padrao imprime 6 casas decimais
    return 0;
}
