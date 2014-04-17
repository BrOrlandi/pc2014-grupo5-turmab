#include <stdio.h>

#define INFINITO 100

/** RETORNA MINIMO ENTRE A E B ***/
float min(float a, float b){
    if(a>b){
        return b;
    }else{
        return a;
    }
}

/*** IMPRIMIR MATRIZ ***/
void imprimiMatriz(int N, float matriz[N][N]){
    int i,j;
    for(i=0;i<N;i++){
        for(j=0;j<N;j++){
            printf("%.2f ",matriz[i][j]);
        }
        printf("\n");
    }
}

/*** CALCULA DISTANCIA ENTRE TODOS OS PONTOS ***/
void floydWarshall(int N, float matriz[N][N]){
    int i, j, k;
    for(i=0;i<N;i++){
        for(j=0;j<N;j++){
            for(k=0;k<N;k++){
                matriz[j][k] = min(matriz[j][k], matriz[j][i] + matriz[i][k]);
            }
        }
    }
    printf("\n");
    imprimiMatriz(N, matriz);
}

int main(){
    int i, j, N;
    scanf("%d ",&N);
    float matriz[N][N], aux;

    //zera a matriz
    for(i=0;i<N;i++){
        for(j=0;j<N;j++){
            matriz[i][j] = 0;
        }
    }

    //carrega a matriz
    for(i = 0; i < N; i++){
        for(j=0;j<N;j++){
            scanf("%f ", &aux);
            if(aux == 0){
                matriz[i][j] = INFINITO;
            }else{
                matriz[i][j] = aux;
            }
        }
    }

    printf("Matriz entrada: \n");
    imprimiMatriz(N, matriz);

    printf("\nMatriz saida: ");
    floydWarshall(N, matriz);

    return 0;
}
