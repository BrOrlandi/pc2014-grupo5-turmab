#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#include <time.h>

#define NTHREADS 4

float entrada[5], medias[NTHREADS], desvios[NTHREADS];
int iteracoes;

/*** INICIO GERADOR DE NUMEROS ALEATORIOS ***/
struct BoxMullerState
{
        double x1, x2, w, y1, y2;
        int useLast;
        struct drand48_data random;
};

void initBoxMullerState(struct BoxMullerState* state)
{
        state->random.__init = 0;
        state->useLast = 0;
        
        struct timeval now;
    gettimeofday(&now, NULL);
    state->random.__x[0] = now.tv_usec;
}

double boxMullerRandom(struct BoxMullerState* state)
{
        double randomNumber;

        if (state->useLast)
        {
                state->y1 = state->y2;
                state->useLast = 0;
        }
        else
        {
                do
                {
                        drand48_r(&state->random, &state->x1);
                        state->x1 = 2.0 * state->x1 - 1.0;
                        drand48_r(&state->random, &state->x2);
                        state->x2 = 2.0 * state->x2 - 1.0;
                        state->w = state->x1 * state->x1 + state->x2 * state->x2;
                }
                while (state->w >= 1.0);

                state->w = sqrt((-2.0 * log(state->w)) / state->w);
                state->y1 = state->x1 * state->w;
                state->y2 = state->x2 * state->w;
                state->useLast = 1;
        }

        return state->y1;
}
/*** FIM GERADOR DE NUMEROS ALEATORIOS ***/


void *calcula(void *parametro){
    //Tipo e inicializando gerador de numeros aleatorios
    struct BoxMullerState state;
    initBoxMullerState(&state);

    float temp, mean, max, stdDev;
    int iteracoesThread, i;
    long threadID;

    threadID = (long)parametro;//recuperou id da thread que esta sendo executada    

    temp = mean = stdDev = 0;

    iteracoesThread = iteracoes/NTHREADS;//Cada thread ira executar parte do total de iteracoes passado pelo usuario

    float valores[iteracoesThread];

    //Realiza iteracoes com calculos
    for(i=0;i<iteracoesThread;i++){

        temp = entrada[0]*exp((entrada[2]-0.5*(entrada[3]*entrada[3]))*entrada[4]+entrada[3]*sqrt(entrada[4])*boxMullerRandom(&state));
    
        //Verifica maximo entre t-E e 0
        if((temp-entrada[1])>0){
            max = temp-entrada[1];
        }else{
            max = 0;
        }
        valores[i] = exp(-entrada[2]*entrada[4])*max;
    }    
    
    //Calcula media da Thread
    for(i=0;i<iteracoesThread;i++){
        mean += valores[i];
    }
    mean = mean/iteracoesThread;
    medias[threadID] = mean;//Armazena media de cada thread em um vetor temporario

    //Calcula PARTE do desvio padrao da Thread
    for(i=0;i<iteracoesThread;i++){
        stdDev += pow(valores[i]-mean,2);
    }
    desvios[threadID] = stdDev;//Avalia o valor intermediario do calculo de desvio padrao de cada thread (conta sera finalizada no main)

    pthread_exit(0);
}

int main(){
    long i;
    float mean, stdDev, confWidth, confMin, confMax;

    mean = stdDev = confWidth = confMin = confMax = 0;

    entrada[0] = entrada[1] = entrada[2] = entrada[3] = entrada[4] = 0;    

    //Realiza leituras das entradas
    scanf("%f",&entrada[0]);
    scanf("%f",&entrada[1]);
    scanf("%f",&entrada[2]);
    scanf("%f",&entrada[3]);
    scanf("%f",&entrada[4]);
    scanf("%d",&iteracoes);

    pthread_t threadID[NTHREADS];    

    for (i = 0; i< NTHREADS ; i++){
        pthread_create (&threadID[i], NULL, calcula, (void*)i);//Cria cada thread com a funcao calcula e passando o respectivo ID
    }

    for (i = 0; i< NTHREADS ; i++){
        pthread_join (threadID[i], NULL);//Espera cada thread terminar de executar
           }

    //Calcula media
    for(i=0;i<NTHREADS;i++){
        mean += medias[i];
    }
    mean = mean/NTHREADS;

    //Calcula desvio padrao
    for(i=0;i<NTHREADS;i++){
        stdDev += desvios[i];
    }
    stdDev = sqrt(stdDev/iteracoes);

    //Calcula intervalo de confianca
    confWidth = 1.96*stdDev/sqrt(iteracoes);

    //Calcula valor inferior intervalo
    confMin = mean - confWidth;

    //Calcula valor superior intervalo
    confMax = mean + confWidth;
    
    printf("S:\t%f\n", entrada[0]);
    printf("E:\t%f\n", entrada[1]);
    printf("r:\t%f\n", entrada[2]);
    printf("sigma:\t%f\n", entrada[3]);
    printf("T:\t%f\n", entrada[4]);
    printf("M:\t%d\n", iteracoes);

    printf("confidence interval: (%f, %f)\n", confMin, confMax);
    
    return 0;
}
