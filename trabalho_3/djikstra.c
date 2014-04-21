#include <stdlib.h>
#include <stdio.h>

#define INFINITY 100000

int n;

int minimumDistance(int dist[n], int set[n]){

	int minimum = INFINITY, m_index, i;

	for(i = 0; i < n; i++){
		if(!set[i] && dist[i] <= minimum)
			minimum = dist[i], m_index = i;
	}

	return m_index;
}

void solution(int dist[n], int node){

	int i, lesser = INFINITY, index = -1;

	printf("\nVertex\tDistance from source: %d\n", node+1);
	for(i = 0; i < n; i++){
		if(dist[i] == INFINITY)
			printf("%d\tinf\n", i+1);

		else {
			printf("%d\t%d\n", i+1, dist[i]);

			if(dist[i] < lesser && dist[i] > 0){
				lesser = dist[i];
				index = i;
			}
		}
	}

	if(index != -1 && lesser < INFINITY && lesser > 0){
		printf("\n-- Shortest path -- \n");
		printf("Vertex: %d - Weight: %d\n\n", index+1, lesser);
	}

	else
		printf("\nNo shortest path!\n\n");
}

void djikstra(int graph[n][n], int node){

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

	solution(dist, node);
}

int main(int argc, char *argv[]){

	int m, i, j, aux_i, aux_j, aux_graph;

	scanf(" %d %d", &n, &m);

	int graph[n][n];

	for(i = 0; i < n; i++)
		for(j = 0; j < n; j++)
			graph[i][j] = 0;

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

	for(i = 0; i < n; i++)
	    djikstra(graph, i);

	return 0;
}
