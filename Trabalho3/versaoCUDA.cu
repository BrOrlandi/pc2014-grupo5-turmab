#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Tamanho do filtro, como definido na especificação: 5x5.
#define TAM_FILTRO 5

#define GRID 1
#define BLOCK 1024
#define TILE_WIDTH 28
#define BLOCK_WIDTH (TILE_WIDTH + (TAM_FILTRO - 1))

// Kernel de convolução
__global__ void convolucao(int *output, int *input, int width, int height){

        // Linha atual
        int idxY = blockIdx.y * TILE_WIDTH + threadIdx.y;

        // Coluna atual
        int idxX = blockIdx.x * TILE_WIDTH + threadIdx.x;

		// utilizacao de memoria compartilhada para diminuir o tempo de acesso aos pixels
        __shared__ int sharedMemory[BLOCK_WIDTH][BLOCK_WIDTH];

        int offset = TAM_FILTRO / 2;
        int sharedX = threadIdx.x, sharedY = threadIdx.y;
        int row_i = idxY - offset;
        int col_i = idxX - offset;

		// Loop que percorre todos os 3 canais de cor para realizar a convolucao
        for(int channelIndex = 0; channelIndex < 3; channelIndex++){

				// calculo de indice necessario, pois houve a transformacao da matriz da imagem em um vetor
                int index = (row_i * width + col_i) * 3 + channelIndex;

				// verifica se esta dentro dos limites
                if((row_i >= 0) && (row_i < height) &&
                        (col_i >= 0) && (col_i < width))
                                sharedMemory[sharedY][sharedX] = input[index];

                else
                        sharedMemory[sharedY][sharedX] = 0;

				// espera a matriz ser populada
                __syncthreads();

                int total = 0;

                // Verifica se valores de linha e coluna pertencem �|  matriz 
                if(sharedY < TILE_WIDTH && sharedX < TILE_WIDTH){
                        for(int i = 0; i < TAM_FILTRO; i++)
                                for(int j = 0; j < TAM_FILTRO; j++)
                                        total += sharedMemory[sharedY+i][sharedX+j];
                        // Média dos pixels
                        if(idxY < height && idxX < width)
                                output[(idxY * width + idxX) * 3 + channelIndex] = total / (TAM_FILTRO * TAM_FILTRO);

                        __syncthreads();
                }
        }
}

int main(int argc, char *argv[]){

        int totalX, totalY, N;

        FILE *arquivo;
        arquivo = fopen(argv[1], "r");

        if(arquivo == NULL){
                printf("Arquivo não pode ser encontrado!\n");
                exit(1);
        }

        // P3
        fseek(arquivo, 2, SEEK_SET);

        // tamanho da imagem
        fscanf(arquivo, "%d %d", &totalX, &totalY);
        N = totalX * totalY * 3;

        //declara matriz de entrada e final
        int *input, *outputFinal;

        input = (int *) malloc(sizeof(int) * N);
        outputFinal = (int *) malloc(sizeof(int) * N);

        //declara matrizes de entrada e saída (CUDA)
        int *dev_input, *dev_output;

        cudaMalloc((void**) &dev_input, N * sizeof(int));
        cudaMalloc((void**) &dev_output, N * sizeof(int));

        // 255
        fseek(arquivo, 4, SEEK_CUR);

        // carrega matriz
        //printf("%d %d\n", totalX, totalY);
        for(int i = 0; i < N; i++){
                fscanf(arquivo, "%d", &input[i]);
                //printf("input[%d]: %d\n", i, input[i]);
        }

        cudaMemcpy(dev_input, input, N * sizeof(int), cudaMemcpyHostToDevice);

		// dimensoes padrao de block e grid
        dim3 dimBlock(BLOCK_WIDTH, BLOCK_WIDTH);
        dim3 dimGrid((totalX-1) / TILE_WIDTH + 1, (totalY-1) / TILE_WIDTH + 1);

        convolucao<<<dimGrid, dimBlock>>>(dev_output, dev_input, totalX, totalY);

        cudaMemcpy(outputFinal, dev_output, N * sizeof(int), cudaMemcpyDeviceToHost);

        //Salva no arquivo de saida
        char str_final[100];

        strcpy(str_final, "out_cuda_");
        strcat(str_final, argv[1]);

        arquivo = fopen(str_final, "w");

        if(arquivo != NULL){
                fprintf(arquivo, "P3\n%d %d\n255\n", totalX, totalY);

                for(int i = 0; i < N; i++)
                        fprintf(arquivo, "%d\n", outputFinal[i]);

                fclose(arquivo);
        }

        else
                printf("Arquivo não pode ser criado!\n");

        free(input);
        free(outputFinal);
        cudaFree(dev_input);
        cudaFree(dev_output);

        return 0;
}
