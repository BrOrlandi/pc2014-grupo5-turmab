#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"

#define INFINITY 100000

int n, m;

int minimumDistance(int dist[n], int set[n]){

	int minimum = INFINITY, m_index, i;

	for(i = 0; i < n; i++){
		if(!set[i] && dist[i] <= minimum)
			minimum = dist[i], m_index = i;
	}

	return m_index;
}

void solution(int dist[n], int node, int rank){

	int i, lesser = INFINITY, index = -1;

	printf("\nVertex\tDistance from source: %d (Rank: %d)\n", node+1, rank);
	for(i = 0; i < n; i++){
		if(dist[i] == INFINITY)
			printf("%d\tinf (rank: %d)\n", i+1, rank);

		else {
			printf("%d\t%d (rank: %d)\n", i+1, dist[i], rank);

			if(dist[i] < lesser && dist[i] > 0){
				lesser = dist[i];
				index = i;
			}
		}
	}

	if(index != -1 && lesser < INFINITY && lesser > 0){
		printf("\n-- Shortest path -- Rank: %d \n", rank);
		printf("Vertex: %d - Weight: %d - Rank: %d\n\n", index+1, lesser, rank);
	}

	else
		printf("\nNo shortest path! Rank: %d\n\n", rank);
}

void djikstra(int graph[n][n], int node, int rank){

	int dist[n], set[n];
	int min, i, j;

	for(i = 0; i < n; i++)
		dist[i] = INFINITY, set[i] = 0;

	dist[node] = 0;

	for(i = 0; i < n-1; i++){
		min = minimumDistance(dist, set);

		set[min] = 1;

		for(j = 0; j < n; j++){
			if(!set[j] && graph[min][j] && dist[min] != INFINITY && dist[min] + graph[min][j] < dist[j])
				dist[j] = dist[min] + graph[min][j];
		}
	}

	solution(dist, node, rank);
}

int main(int argc, char *argv[]){

	int tag, rank, size, m, i, j, aux_i, aux_j, aux_graph;
	MPI_Status status;

	scanf(" %d %d", &n, &m);

	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	//MPI_Bcast(graph, n * n, MPI_INT, 0, MPI_COMM_WORLD);

	int graph[n][n];

	if(rank == 0){
		for(i = 0; i < n; i++)
			for(j = 0; j < n; j++)
				graph[i][j] = 0;

		for(i = 0; i < m; i++){
			scanf(" %d %d %d", &aux_i, &aux_j, &aux_graph);
			graph[aux_i-1][aux_j-1] = aux_graph;
	 	}

		for(i = 1; i < size; i++)
			MPI_Send(graph, n * n, MPI_INT, i, 1, MPI_COMM_WORLD);

		if(n % 2)
			djikstra(graph, n-1, 0);
	}

	else
		MPI_Recv(graph, n * n, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

	for(i = (n/size) * rank; i < (n/size) + ((n/size) * rank); i++)
	    djikstra(graph, i, rank);

	MPI_Finalize();

	return 0;
}
