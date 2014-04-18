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
    int rank, size, n, m, p,i,j,k;

    MPI_Status status;

    //Inicia MPI
    MPI_Init(&argc,&argv);

    //Pega o valor do rank e o total de processos executando
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

    //processo principal
	if(rank == 0){
        scanf("%d %d %d ", &n, &m, &p);
        int A[n][m], B[m][p], C[n][p];
        //carrega as matrizes
        allocMatrices(n, m, p, A, B, C);

        for(i=0;i<size;i++){
            //Envia valores de n,m,p e as matrizes
            MPI_Send(&n, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&m, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&p, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&A[0][0], n*m, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&B[0][0], m*p, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&C[0][0], n*p, MPI_INT, i, 1, MPI_COMM_WORLD);
        }
        //Calcula parte da matriz
        for(i=rank*(n/size);i<(rank*(n/size)+(n/size));i++){
            for(j=0;j<p;j++){
                for(k=0;k<m;k++){
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }

        //recebe as partes já calculadas
        for(i = 1;i<size;i++){
            if(i == size - 1){
                for(j=rank*(n/size);j<n-(rank*(n/size)+(n/size));j++){
                    MPI_Recv(&C[i], p, MPI_INT, i, 1, MPI_COMM_WORLD, &status);
                }
            }else{
                for(j=rank*(n/size);j<(rank*(n/size)+(n/size));j++){
                    MPI_Recv(&C[i], p, MPI_INT, i, 1, MPI_COMM_WORLD, &status);
                }
            }
        }
        printMatrix(n,p,C);

    //processos "filhos"
	}else{
	    //Recebe os valores de n,m,p e as matrizes
        MPI_Recv(&n, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&m, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&p, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        int A[n][m], B[m][p], C[n][p];
        MPI_Recv(&A[0][0], n*m, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&B[0][0], m*p, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&C[0][0], n*p, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

        //Calcula parte da matriz
        if(rank == size-1){
            //Se for ultimo processo
            for(i=rank*(n/size);i<(rank*(n/size)+(n/size));i++){
                for(j=0;j<p;j++){
                    for(k=0;k<m;k++){
                        C[i][j] += A[i][k] * B[k][j];
                    }
                }
            }
        }else{
            for(i=rank*(n/size);i<(rank*(n/size)+(n/size));i++){
                for(j=0;j<p;j++){
                    for(k=0;k<m;k++){
                        C[i][j] += A[i][k] * B[k][j];
                    }
                }
            }
        }
        for(i=rank*(n/size);i<(rank*(n/size)+(n/size));i++){
                MPI_Send(&C[i], p, MPI_INT, 0, 1, MPI_COMM_WORLD);
        }
	}

    //Finaliza MPI
    MPI_Finalize();
    return 0;
}
