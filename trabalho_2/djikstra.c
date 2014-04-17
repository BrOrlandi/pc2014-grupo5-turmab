#include <stdlib.h>
#include <stdio.h>

#define N 9
#define INF 100000

int minimumDistance(int dist[N], int set[N]){

	int minimum = INF, m_index, i;

	for(i = 0; i < N; i++){
		if(!set[i] && dist[i] <= minimum)
			minimum = dist[i], m_index = i;
	}

	return m_index;
}

void solution(int dist[N]){

	int i;

	printf("Vertex\tDistance from source\n");
	for(i = 0; i < N; i++)
		printf("%d\t%d\n", i, dist[i]);
}

void djikstra(int graph[N][N], int node){

	int dist[N], set[N];
	int min, i, j;

	for(i = 0; i < N; i++)
		dist[i] = INF, set[i] = 0;

	dist[node] = 0;

	for(i = 0; i < N-1; i++){
		min = minimumDistance(dist, set);

		set[min] = 1;

		for(j = 0; j < N; j++){
			if(!set[j] && graph[min][j] && dist[min] != INF && dist[min] + graph[min][j] < dist[j])
				dist[j] = dist[min] + graph[min][j];
		}
	}

	solution(dist);
}

int main(int argc, char *argv[]){
	
	int graph[N][N] = {{0, 4, 0, 0, 0, 0, 0, 8, 0},
                      {4, 0, 8, 0, 0, 0, 0, 11, 0},
                      {0, 8, 0, 7, 0, 4, 0, 0, 2},
                      {0, 0, 7, 0, 9, 14, 0, 0, 0},
                      {0, 0, 0, 9, 0, 10, 0, 0, 0},
                      {0, 0, 4, 0, 10, 0, 2, 0, 0},
                      {0, 0, 0, 14, 0, 2, 0, 1, 6},
                      {8, 11, 0, 0, 0, 0, 1, 0, 7},
                      {0, 0, 2, 0, 0, 0, 6, 7, 0}
                     };
 
    djikstra(graph, 0);

	return 0;
}
