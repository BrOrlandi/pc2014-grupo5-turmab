#include <stdio.h>
#include <stdlib.h>

#define TAM 10

typedef enum{
	LABELED, UNLABELED, SCANNED
}State;

/*** Structs ***/

typedef struct edge{
    struct node* tail;
	struct node* head;
	double length;
	double delta;
}Edge;

typedef struct node{
    struct node * parent;
	struct node * leftSibling, * rightSibling;
	struct node * children;
	struct node * pred;
	int data;
	double key;
	int rank;
	Edge *incomingEdges[TAM];
	int tamIncoming;
	Edge *outgoingEdges[TAM];
	int tamOutgoing;
	State state;
}Node;

typedef struct{
    Node* rootListByRank[100];
    Node * minRoot;
}FibonacciHeap;

/*** Cabeçalhos ***/
void createEdge(Edge *aux, Node* tail, Node* head, double length);
void createNodeArgs(Node *aux, int data, double key);
void createNodeNotArgs(Node *aux);
int addChild(Node *aux, Node *node);
int addSibling(Node *aux, Node *node);
int removeNode(Node *aux);
void addIncomingEdge(Node *aux, Edge *edge);
void addOutgoingEdge(Node *aux, Edge * edge);
Node* leftMostSibling(Node *aux);
Node* rightMostSibling(Node *aux);
void FibonacciHeapNotArgs(FibonacciHeap *aux);
void fibonacciHeapArgs(FibonacciHeap *aux, Node *root);
int isEmpty(FibonacciHeap *aux);
int insertVertex(FibonacciHeap *aux, Node * node);
Node* findMin(FibonacciHeap *aux);
Node* deleteMin(FibonacciHeap *aux);
int link(FibonacciHeap *aux, Node* root);
void decreaseKey(FibonacciHeap *aux, double delta, Node* vertex);

/*** Funções ***/

/****** Parte Edges ******/
void createEdge(Edge *aux, Node* tail, Node* head, double length)
{
    aux->tail = tail;
	aux->head = head;
	aux->length = length;
}
/****** Fim Parte Edges ******/

/****** Parte Node ******/
void createNodeArgs(Node *aux, int data, double key)
{
	aux->key = key;
	aux->data = data;
	aux->parent = NULL;
	aux->children = NULL;
	aux->leftSibling = NULL;
	aux->rightSibling = NULL;
	aux->pred = NULL;
	aux->rank = 0;
	aux->state = UNLABELED;
	aux->tamIncoming = 0;
	aux->tamOutgoing = 0;
}

void createNodeNotArgs(Node *aux)
{
	aux->parent = NULL;
	aux->children = NULL;
	aux->leftSibling = NULL;
	aux->rightSibling = NULL;
	aux->pred = NULL;
	aux->rank = 0;
	aux->state = UNLABELED;
	aux->tamIncoming = 0;
	aux->tamOutgoing = 0;
}

int addChild(Node *aux, Node *node)
{
	if(aux->children != NULL)
        addSibling(aux->children, node);
	else
	{
		aux->children = node;
		node->parent = aux;
		aux->rank = 1;
	}

	return 1;
}

int addSibling(Node *aux, Node *node)
{
	Node* temp = rightMostSibling(aux);
	if(temp == NULL)
		return 0;

	temp->rightSibling = node;
	node->leftSibling = temp;
	node->parent = aux->parent;
	node->rightSibling = NULL;

	if(aux->parent)
		aux->parent->rank++;

	return 1;
}

int removeNode(Node *aux)
{
	if(aux->parent)
	{
		aux->parent->rank--;
		if(aux->leftSibling)
			aux->parent->children = aux->leftSibling;
		else if(aux->rightSibling)
			aux->parent->children = aux->rightSibling;
		else
			aux->parent->children = NULL;
	}

	if(aux->leftSibling)
		aux->leftSibling->rightSibling = aux->rightSibling;
	if(aux->rightSibling)
		aux->rightSibling->leftSibling = aux->leftSibling;

	aux->leftSibling = NULL;
	aux->rightSibling = NULL;
	aux->parent = NULL;

	return 1;
}

void addIncomingEdge(Node *aux, Edge *edge)
{
	aux->incomingEdges[aux->tamIncoming] = edge;
	aux->tamIncoming++;
}

void addOutgoingEdge(Node *aux, Edge * edge)
{
	aux->outgoingEdges[aux->tamOutgoing] = edge;
	aux->tamOutgoing++;
}

Node* leftMostSibling(Node *aux)
{
	if(aux == NULL)
		return NULL;

	Node* temp = aux;
	while(temp->leftSibling)
		temp = temp->leftSibling;
	return temp;
}

Node* rightMostSibling(Node *aux)
{
	if(aux == NULL)
		return NULL;

	Node* temp = aux;
	while(temp->rightSibling)
		temp = temp->rightSibling;
	return temp;
}
/****** Fim Parte Node ******/

/****** Parte Fibonacci Heap ******/
void fibonacciHeapNotArgs(FibonacciHeap *aux)
{
	aux->minRoot = NULL;
}

void fibonacciHeapArgs(FibonacciHeap *aux, Node *root)
{
	aux->minRoot = root;
	aux->minRoot->parent = NULL;
	aux->minRoot->children = NULL;
	aux->minRoot->leftSibling = NULL;
	aux->minRoot->rightSibling = NULL;
	aux->minRoot->rank = 0;
}

int isEmpty(FibonacciHeap *aux)
{
	return (aux->minRoot == NULL);
}

int insertVertex(FibonacciHeap *aux, Node * node)
{
	if(node == NULL)
		return 0;

	if(aux->minRoot == NULL)
		aux->minRoot = node;
	else
	{
	    addSibling(aux->minRoot,node);
		if(aux->minRoot->key > node->key)
			aux->minRoot = node;
	}
	return 1;
}

Node* findMin(FibonacciHeap *aux)
{
	return aux->minRoot;
}

Node* deleteMin(FibonacciHeap *aux)
{
	Node *temp = leftMostSibling(aux->minRoot->children);
	Node *nextTemp = NULL;

	// Adding Children to root list
	while(temp != NULL)
	{
		nextTemp = temp->rightSibling; // Save next Sibling
		removeNode(temp);
		addSibling(aux->minRoot,temp);
		temp = nextTemp;
	}

	// Select the left-most sibling of minRoot
	temp = leftMostSibling(aux->minRoot);

	// Remove minRoot and set it to any sibling, if there exists one
	if(temp == aux->minRoot)
	{
		if(aux->minRoot->rightSibling)
			temp = aux->minRoot->rightSibling;
		else
		{
			// Heap is obviously empty
			Node* out = aux->minRoot;
			removeNode(aux->minRoot);
			aux->minRoot = NULL;
			return out;
		}
	}
	Node* out = aux->minRoot;
	removeNode(aux->minRoot);
	aux->minRoot = temp;

	// Initialize list of roots
	int i;
	for(i=0; i<100; i++)
		aux->rootListByRank[i] = NULL;

	while(temp)
	{
		// Check if key of current vertex is smaller than the key of minRoot
		if(temp->key < aux->minRoot->key)
		{
			aux->minRoot = temp;
		}

		nextTemp = temp->rightSibling;
		link(aux, temp);
		temp = nextTemp;
	}

	return out;
}

int link(FibonacciHeap *aux, Node* root)
{
	// Insert Vertex into root list
	if(aux->rootListByRank[root->rank] == NULL)
	{
		aux->rootListByRank[root->rank] = root;
		return 0;
	}
	else
	{
		// Link the two roots
		Node* linkVertex = aux->rootListByRank[root->rank];
		aux->rootListByRank[root->rank] = NULL;

		if(root->key < linkVertex->key || root == aux->minRoot)
		{
		    removeNode(linkVertex);
		    addChild(root,linkVertex);
			if(aux->rootListByRank[root->rank] != NULL)
				link(aux, root);
			else
				aux->rootListByRank[root->rank] = root;
		}
		else
		{
		    removeNode(root);
		    addChild(linkVertex, root);
			if(aux->rootListByRank[linkVertex->rank] != NULL)
				link(aux, linkVertex);
			else
				aux->rootListByRank[linkVertex->rank] = linkVertex;
		}
		return 1;
	}
}

void decreaseKey(FibonacciHeap *aux, double delta, Node* vertex)
{
	vertex->key = delta;

	if(vertex->parent != NULL) // The vertex has a parent
	{
		// Remove vertex and add to root list
		removeNode(vertex);
		addSibling(aux->minRoot, vertex);
	}
	// Check if key is smaller than the key of minRoot
	if(vertex->key < aux->minRoot->key)
		aux->minRoot = vertex;
}

