/*
	Dijkstra - versão sequencial
	Autores: Bruno Henrique Orlandi		NUSP: 7573378
			 Gustavo Blanco				NUSP: 7656234
			 Nihey Takizawa				NUSP: 7572912
			 Rafael Ribaldo				NUSP: 6792270
*/

#include <stdlib.h>
#include <stdio.h>

#define INFINITY 10000000

int n, sp = INFINITY;

int minimumDistance(int dist[n], int set[n]){

	int minimum = INFINITY, m_index;
	int i;

	// Descobrimento da distância entre um vértice e seus adjacentes
	for(i = 0; i < n; i++){
		if(!set[i] && dist[i] <= minimum)
			minimum = dist[i], m_index = i;
	}

	return m_index;
}

void solution(int dist[n], int node){

	int i, lesser = 0, index = -1;

	// A solução envolve a escolha da menor distância entre as maiores
	for(i = 0; i < n; i++){
		if(dist[i] > lesser && dist[i] < INFINITY){
			lesser = dist[i];
			index = i;
		}
	}

	if(index != -1 && lesser < INFINITY && lesser > 0){
		if(lesser < sp)
			sp = lesser;
	}
}

void djikstra(int graph[n][n], int node){

	int dist[n], set[n];
	int min;
	int i, j;

	// Inicialização das distâncias
	for(i = 0; i < n; i++)
		dist[i] = INFINITY, set[i] = 0;

	dist[node] = 0;

	for(i = 0; i < n-1; i++){

		// Calculo da distância mínima
		min = minimumDistance(dist, set);

		set[min] = 1;

		// Descoberta do melhor caminho entre dois vértices
		for(j = 0; j < n; j++){
			if(!set[j] && graph[min][j] && dist[min] != INFINITY && dist[min] + graph[min][j] < dist[j])
				dist[j] = dist[min] + graph[min][j];
		}
	}

	solution(dist, node);
}

int main(int argc, char *argv[]){

	int m, i, j, aux_i, aux_j, aux_graph;

	// Leitura do número de vértices e arestas
	scanf(" %d %d", &n, &m);

	int graph[n][n];

	// Inicialização do grafo
	for(i = 0; i < n; i++)
		for(j = 0; j < n; j++)
			graph[i][j] = 0;

	// Leitura do peso de cada aresta e seus respectivos vértices
	for(i = 0; i < m; i++){
		scanf(" %d %d %d", &aux_i, &aux_j, &aux_graph);
		graph[aux_i-1][aux_j-1] = aux_graph;
		graph[aux_j-1][aux_i-1] = aux_graph;
 	}

	// Chamada da função para o cálculo do menor caminho partindo de cada vértice
	for(i = 0; i < n; i++)
	    djikstra(graph, i);

	printf("%d\n", sp);

	return 0;
}
