/*
	Dijkstra - versão paralela
	Autores: Bruno Henrique Orlandi		NUSP: 7573378
			 Gustavo Blanco				NUSP: 7656234
			 Nihey Takizawa				NUSP: 7572912
			 Rafael Ribaldo				NUSP: 6792270
*/

#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"

#define INFINITY 1000000

int n, sp = INFINITY;

int minimumDistance(int dist[n], int set[n]){

	int minimum = INFINITY, m_index, i;

	// Descobrimento da distância entre um vértice e seus adjacentes
	for(i = 0; i < n; i++){
		if(!set[i] && dist[i] <= minimum)
			minimum = dist[i], m_index = i;
	}

	return m_index;
}

void solution(int dist[n], int node, int rank){

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

void djikstra(int graph[n][n], int node, int rank){

	int dist[n], set[n];
	int min, i, j;

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

	solution(dist, node, rank);
}

int main(int argc, char *argv[]){

	int index = 1, m, tag, rank, size, i, j, aux_i, aux_j, aux_graph, final_sp;
	MPI_Status status;

	// Leitura do número de vértices e arestas
	scanf(" %d %d", &n, &m);

	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	// Broadcast do número de vértices para todos os nós para a alocação da matriz
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

	// Verificão necessária caso o usuário extrapole o número de processos permitido para um determinado número de vértices
	if(size > n){
		if(rank == 0)
			printf("Erro! Total de processos não pode ser maior que o total de vértices!\n");
	}

	else {
		// Broadcast da variável correspondente ao caminho de um vértice até seu ponto mais distante
		MPI_Bcast(&sp, 1, MPI_INT, 0, MPI_COMM_WORLD);
		int graph[n][n];

		if(rank == 0){

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

			// Envio da matriz de vértices para cada nó
			for(i = 1; i < size; i++)
				MPI_Send(graph, n * n, MPI_INT, i, 1, MPI_COMM_WORLD);
			
			// Caso o número de vértices for ímpar, o nó 0 se responsabiliza em computar o vértice restante
			if(n % 2)
				djikstra(graph, n-1, 0);
		}

		// Recebimento da matriz de vértices do nó 0
		else
			MPI_Recv(graph, n * n, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

		// Chamada da função, para cada nó, para o cálculo do menor caminho partindo de cada vértice
		for(i = (n/size) * rank; i < (n/size) + ((n/size) * rank); i++)
			djikstra(graph, i, rank);

		// Redução das variáveis 'sp' para a escolha do seu valor mínimo
		MPI_Reduce(&sp, &final_sp, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

		if(rank == 0)
			printf("%d\n", final_sp);
	}

	MPI_Finalize();	

	return 0;
}
