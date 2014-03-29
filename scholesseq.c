#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#include <time.h>

float entrada[5];
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

void calcula(){
    //Tipo e inicializando gerador de numeros aleatorios
    struct BoxMullerState state;
    initBoxMullerState(&state);

    float valores[iteracoes];
    float temp, mean, max, stdDev, confWidth, confMin, confMax;
    int i;

    temp = mean = stdDev = confWidth = confMin = confMax = 0;

    //Realiza iteracoes com calculos
    for(i=0;i<iteracoes;i++){

        temp = entrada[0]*exp((entrada[2]-0.5*(entrada[3]*entrada[3]))*entrada[4]+entrada[3]*sqrt(entrada[4])*boxMullerRandom(&state));

        //Verifica maximo entre t-E e 0
        if((temp-entrada[1])>0){
            max = temp-entrada[1];
        }else{
            max = 0;
        }
        valores[i] = exp(-entrada[2]*entrada[4])*max;
    }    
    
    mean = 0;
    //Calcula media
    for(i=0;i<iteracoes;i++){
        mean += valores[i];
    }
    mean = mean/iteracoes;

    //Calcula desvio padrao
    for(i=0;i<iteracoes;i++){
        stdDev += pow(valores[i]-mean,2);
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
}

int main(){
    entrada[0] = entrada[1] = entrada[2] = entrada[3] = entrada[4] = 0;    

    //Realiza leituras das entradas
    scanf("%f",&entrada[0]);
    scanf("%f",&entrada[1]);
    scanf("%f",&entrada[2]);
    scanf("%f",&entrada[3]);
    scanf("%f",&entrada[4]);
    scanf("%d",&iteracoes);

    calcula();
    
    return 0;
}
