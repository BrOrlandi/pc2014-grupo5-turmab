/**
Bruno Henrique Orlandi
Gustavo Blanco              7656234
Nihey Takizawa
Rafael Ribaldo
**/

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

//Le as entradas e preenche as matrizes
void allocMatrices(int n, int m, int p,int A[n][m], int B[m][p], int C[n][p]){
	int i, j;
	for(i = 0; i < n; i++){
		for(j = 0; j < m; j++){
			scanf(" %d", &A[i][j]);
		}
	}
	for(i = 0; i < m; i++){
		for(j = 0; j < p; j++)
			scanf(" %d", &B[i][j]);
	}
	for(i = 0; i < m; i++){
		for(j = 0; j < p; j++)
			C[i][j] = 0;
	}
}

//Imprime a matriz
void printMatrix(int x, int y, int A[x][y]){
    int i, j;
    for(i=0;i<x;i++){
        for(j=0;j<y;j++){
            printf("%d ",A[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char **argv){
    int rank, size, n, m, p,i,j,k, mediaLinhas, linhas, extra, offset, rc;

    MPI_Status status;

    //Inicia MPI
    MPI_Init(&argc,&argv);

    //Pega o valor do rank e o total de processos executando
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

    //Verifica o total de processos executando
	if (size < 2 ) {
        printf("É necessário pelo menos 2 tarefas!\n");
        MPI_Abort(MPI_COMM_WORLD, rc);
        exit(1);
    }

    //processo principal
	if(rank == 0){
        //Leitura das dimensoes
        scanf("%d %d %d ", &n, &m, &p);
        int A[n][m], B[m][p], C[n][p];
        //carrega as matrizes
        allocMatrices(n, m, p, A, B, C);
        mediaLinhas = n/(size-1);
        extra = n%(size-1);
        offset = 0;

        for(i=1;i<size;i++){
            if(i <= extra){
                linhas = mediaLinhas+1;
            }else{
                linhas = mediaLinhas;
            }
            //Envia valores de n,m,p e as matrizes
            MPI_Send(&n, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&m, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&p, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&offset, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&linhas, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&A[offset][0], linhas*m, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&B, m*p, MPI_INT, i, 1, MPI_COMM_WORLD);
            offset = offset + linhas;
        }

        //recebe as partes já calculadas
        for(i = 1;i<size;i++){
            MPI_Recv(&offset, 1, MPI_INT, i, 2, MPI_COMM_WORLD, &status);
            MPI_Recv(&linhas, 1, MPI_INT, i, 2, MPI_COMM_WORLD, &status);
            MPI_Recv(&C[offset][0], linhas*p, MPI_INT, i, 2, MPI_COMM_WORLD, &status);
        }
        printMatrix(n,p,C);

    //processos "filhos"
	}else{
	    //Recebe os valores de n,m,p e as matrizes
	    MPI_Recv(&n, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&m, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&p, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        int A[n][m], B[m][p], C[n][p];
        MPI_Recv(&offset, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&linhas, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&A, linhas*m, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&B, m*p, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

        //Calcula parte da matriz
        for(k=0;k<p;k++)
            for(i=0;i<linhas;i++)
            {
                C[i][k] = 0;
                for(j=0;j<m;j++)
                    C[i][k] += A[i][j]*B[j][k];
            }
        //Envia os valores calculados
        MPI_Send(&offset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        MPI_Send(&linhas, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        MPI_Send(&C, linhas*p, MPI_INT, 0, 2, MPI_COMM_WORLD);
	}
    //Finaliza MPI
    MPI_Finalize();
}
