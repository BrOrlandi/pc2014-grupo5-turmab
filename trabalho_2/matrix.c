#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"

void allocMatrices(int **A, int **B, int **C, int n, int m, int p){

	int i, j;

	A = (int **) malloc(sizeof(int *) * n);
	B = (int **) malloc(sizeof(int *) * m);
	C = (int **) malloc(sizeof(int *) * n);

	for(i = 0; i < n; i++){
		A[i] = (int *) malloc(sizeof(int) * m);
		C[i] = (int *) malloc(sizeof(int) * p);

		for(j = 0; j < m; j++){
			scanf(" %d", &A[i][j]);
			C[i][j] = 0;
		}
	}

	for(i = 0; i < m; i++){
		B[i] = (int *) malloc(sizeof(int) * p);

		for(j = 0; j < p; j++)
			scanf(" %d", &B[i][j]);
	}
}

void freeMatrices(int **A, int **B, int **C, int n, int m){

	int i, j;

	for(i = 0; i < n; i++){
		free(A[i]);
		free(C[i]);
	}

	for(i = 0; i < m; i++)
		free(B[i]);

	free(A);
	free(B);
	free(C);
}

int main(int argc, char *argv[]){

	int **A, **B, **C;
	int n, m, p, i, j, k, rank, size, tag;
	MPI_Status status;

	scanf(" %d %d %d", &n, &m, &p);

	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if(rank == 0){
// Precisa testar o Broadcast. No momento não está funcionando (não sei por quê).
// Broadcast parece que tem alguma otimização em relação do Send dentro do for.
//		MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
		allocMatrices(A, B, C, n, m, p);
		for(i = 1; i < size; i++){
			MPI_Send(&n, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
			MPI_Send(&m, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
			MPI_Send(&p, 1, MPI_INT, i, tag, MPI_COMM_WORLD);

			MPI_Send(A[i * n / size], m, MPI_INT, i, tag, MPI_COMM_WORLD);
			MPI_Send(B, m * p, MPI_INT, i, tag, MPI_COMM_WORLD);
			MPI_Send(C[i * n / size], p, MPI_INT, i, tag, MPI_COMM_WORLD);
		}
	}

	else {	

		MPI_Recv(&n, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
		MPI_Recv(&m, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
		MPI_Recv(&p, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);

		MPI_Recv(A, m, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
		MPI_Recv(B, m * p, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
		MPI_Recv(C, p, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
	}

	for(j = 0; j < p; j++)
		for(k = 0; k < m; k++)
			C[rank][j] += A[rank][k] * B[k][j];

	if(rank != 0)
		MPI_Send(C, p, MPI_INT, 0, tag, MPI_COMM_WORLD);

	else {
		for(i = 1; i < size; i++)
			MPI_Recv(C[i * n / size], p, MPI_INT, i, tag, MPI_COMM_WORLD, &status);

		for(i = 0; i < n; i++){
			for(j = 0; j < p; j++)
				printf("%d ", C[i][j]);
			printf("\n");
		}

		freeMatrices(A, B, C, n, m);
	}

	MPI_Finalize();

	return 0;
}
