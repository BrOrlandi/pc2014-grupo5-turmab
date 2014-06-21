#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Le a entrada e divide nas 3 matrizes
void carregaMatriz(FILE *arquivo, int totalX, int totalY, int **matrizR,int **matrizG,int **matrizB){
	int i,j;

	for(i=0;i<totalX;i++)
		for(j=0;j<totalY;j++)
			fscanf(arquivo, "%d %d %d ",&matrizR[i][j],&matrizG[i][j],&matrizB[i][j]);
}

//Imprimi matriz na tela
void imprimiMatriz(int totalX, int totalY, int **matriz){
	int i,j;

	for(i=0;i<totalX;i++){
		for(j=0;j<totalY;j++)
			printf("%d ",matriz[i][j]);

		printf("\n");

	}
}

//Salva 3 matrizes no arquivo .ppm seguindo a formatacao necessaria
void salvaMatriz(FILE *arquivo, int totalX, int totalY, int **matrizR, int **matrizG, int **matrizB){

	int i,j;

	for(i=0;i<totalX;i++)
		for(j=0;j<totalY;j++)
			fprintf(arquivo, "%d\n%d\n%d\n", matrizR[i][j], matrizG[i][j], matrizB[i][j]);

	fclose(arquivo);
}

//Realiza a convolucao da matriz de entrada e salva o resultado na matriz de saida
void convolucao(int totalX, int totalY, int **matrizEntrada, int **matrizSaida){
	int i,j,xi,yj, total;

	for(i=0;i<totalX;i++){
		for(j=0;j<totalY;j++){
			total = 0;

			for(xi=i-2;xi<=i+2;xi++){
				for(yj=j-2;yj<=j+2;yj++){

					//Verifica se coordenadas sao validas e assume valor zero caso nao sejam
					if((xi < 0) || (xi >= totalX) || (yj < 0) || ((yj >= totalY)))
						total += 0;

					else
					total += matrizEntrada[xi][yj];
				}

			}

			total = total/25;
			matrizSaida[i][j] = total;
		}
	}
}

int main(int argc, char *argv[]){
	FILE *arquivo = fopen(argv[1], "r");
	int totalX,totalY, i;

	if(arquivo == NULL){
		printf("Arquivo não pode ser encontrado!\n");
		exit(1);
	}

	// P3
	fseek(arquivo, 2, SEEK_SET);

	// tamanho da image
	fscanf(arquivo, "%d %d", &totalX, &totalY);

	// 255
	fseek(arquivo, 4, SEEK_CUR);

	//declara matrizes de entrada
	int **matrizR, **matrizG, **matrizB;

	matrizR = (int **) malloc(sizeof(int *) * totalX);
	matrizG = (int **) malloc(sizeof(int *) * totalX);
	matrizB = (int **) malloc(sizeof(int *) * totalX);

	for(i = 0; i < totalX; i++){
		matrizR[i] = (int *) malloc(sizeof(int) * totalY);
		matrizG[i] = (int *) malloc(sizeof(int) * totalY);
		matrizB[i] = (int *) malloc(sizeof(int) * totalY);
	}

	//declara matrizes de saida
	int **matrizSaidaR, **matrizSaidaG, **matrizSaidaB;

	matrizSaidaR = (int **) malloc(sizeof(int *) * totalX);
	matrizSaidaG = (int **) malloc(sizeof(int *) * totalX);
	matrizSaidaB = (int **) malloc(sizeof(int *) * totalX);

	for(i = 0; i < totalX; i++){
		matrizSaidaR[i] = (int *) malloc(sizeof(int) * totalY);
		matrizSaidaG[i] = (int *) malloc(sizeof(int) * totalY);
		matrizSaidaB[i] = (int *) malloc(sizeof(int) * totalY);
	}

	//carrega todas as matrizes
	carregaMatriz(arquivo, totalX, totalY, matrizR, matrizG, matrizB);

	fclose(arquivo);

	//faz convolucao em cada uma das 3 matrizes
	convolucao(totalX, totalY, matrizR, matrizSaidaR);
	convolucao(totalX, totalY, matrizG, matrizSaidaG);
	convolucao(totalX, totalY, matrizB, matrizSaidaB);

	char str_final[100];

	strcpy(str_final, "out_seq_");
	strcat(str_final, argv[1]);

	arquivo = fopen(str_final, "w");

	if(arquivo == NULL){
		printf("Arquivo não pode ser encontrado!\n");
		exit(1);
	}

	fprintf(arquivo, "P3\n%d %d\n255\n", totalX, totalY);

	//Salva no arquivo de saida
	salvaMatriz(arquivo, totalX, totalY, matrizSaidaR, matrizSaidaG, matrizSaidaB);

	for(i = 0; i < totalX; i++){
		free(matrizR[i]);
		free(matrizG[i]);
		free(matrizB[i]);
		free(matrizSaidaR[i]);
		free(matrizSaidaG[i]);
		free(matrizSaidaB[i]);
	}

	free(matrizR);
	free(matrizG);
	free(matrizB);
	free(matrizSaidaR);
	free(matrizSaidaG);
	free(matrizSaidaB);

	return 0;
}

