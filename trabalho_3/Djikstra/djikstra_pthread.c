#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define INFINITY 100000
#define N_THREADS 2

pthread_t threads[N_THREADS];
int n, **graph;

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
		printf("\n-- Shortest path --  (rank: %d)\n", rank);
		printf("Vertex: %d - Weight: %d (rank: %d)\n\n", index+1, lesser, rank);
	}

	else
		printf("\nNo shortest path! (rank: %d)\n\n", rank);
}

void *djikstra(void *offset){

	int dist[n], set[n];
	int min, i, j;

	int off = *((int *) offset);
	int div = n / N_THREADS;
	int from = div * off;
	int to = from + div;
	int index;

	for(index = from; index < to; index++){

		for(i = 0; i < n; i++)
			dist[i] = INFINITY, set[i] = 0;

		dist[index] = 0;

		for(i = 0; i < n-1; i++){
			min = minimumDistance(dist, set);

			set[min] = 1;

			for(j = 0; j < n; j++){
				if(!set[j] && graph[min][j] && dist[min] != INFINITY && dist[min] + graph[min][j] < dist[j])
					dist[j] = dist[min] + graph[min][j];
			}
		}

		solution(dist, index, off);
	}

	pthread_exit(NULL);
}

int main(int argc, char *argv[]){

	int rc, m, i, j, aux_i, aux_j, aux_graph;

	scanf(" %d %d", &n, &m);

	graph = (int **) malloc(sizeof(int *) * n);

	for(i = 0; i < n; i++){
		graph[i] = (int *) malloc(sizeof(int) * n);

		for(j = 0; j < n; j++)
			graph[i][j] = 0;
	}

	for(i = 0; i < m; i++){
		scanf(" %d %d %d", &aux_i, &aux_j, &aux_graph);
		graph[aux_i-1][aux_j-1] = aux_graph;
 	}

	printf("\nGraph:\n");
	for(i = 0; i < n; i++){
		for(j = 0; j < n; j++)
			printf("%d ", graph[i][j]);
		printf("\n");
	}

	for(i = 0; i < N_THREADS; i++){
		// Criação das threads
		rc = pthread_create(&threads[i], NULL, djikstra, &i);

		if(rc){
			printf("ERROR: %d\n", rc);
			return -1;
		}
	}

	// Junção das threads quando completarem sua execução (em ordem de criação)
	for(i = 0; i < N_THREADS; i++)
		pthread_join(threads[i], NULL);

	return 0;
}
