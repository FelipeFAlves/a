#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "grafo.h"

typedef struct {
  int vertex;
  int priority;
} PQNode;

typedef struct {
  PQNode *nodes;
  int size;
  int capacity;
} PriorityQueue;

PriorityQueue* createPriorityQueue(int capacity) {
  PriorityQueue *pq = (PriorityQueue *)malloc(sizeof(PriorityQueue));
  pq->nodes = (PQNode *)malloc(sizeof(PQNode) * capacity);
  pq->size = 0;
  pq->capacity = capacity;
  return pq;
}

void swap(PQNode *a, PQNode *b) {
  PQNode temp = *a;
  *a = *b;
  *b = temp;
}

void heapifyUp(PriorityQueue *pq, int index) {
  while (index > 0) {
    int parent = (index - 1) / 2;
    if (pq->nodes[index].priority >= pq->nodes[parent].priority) {
      break;
    }
    swap(&pq->nodes[index], &pq->nodes[parent]);
    index = parent;
  }
}

void heapifyDown(PriorityQueue *pq, int index) {
  int left, right, smallest;
  while (1) {
    left = 2 * index + 1;
    right = 2 * index + 2;
    smallest = index;

    if (left < pq->size && pq->nodes[left].priority < pq->nodes[smallest].priority) {
      smallest = left;
    }
    if (right < pq->size && pq->nodes[right].priority < pq->nodes[smallest].priority) {
      smallest = right;
    }
    if (smallest == index) {
      break;
    }
    swap(&pq->nodes[index], &pq->nodes[smallest]);
    index = smallest;
  }
}

void insert(PriorityQueue *pq, int vertex, int priority) {
  if (pq->size == pq->capacity) {
    pq->capacity *= 2;
    pq->nodes = (PQNode *)realloc(pq->nodes, sizeof(PQNode) * pq->capacity);
  }
  pq->nodes[pq->size].vertex = vertex;
  pq->nodes[pq->size].priority = priority;
  heapifyUp(pq, pq->size);
  pq->size++;
}

PQNode extractMin(PriorityQueue *pq) {
  PQNode minNode = pq->nodes[0];
  pq->nodes[0] = pq->nodes[pq->size - 1];
  pq->size--;
  heapifyDown(pq, 0);
  return minNode;
}

int isEmpty(PriorityQueue *pq) {
  return pq->size == 0;
}

void freePriorityQueue(PriorityQueue *pq) {
  free(pq->nodes);
  free(pq);
}


void dijkstra(Grafo G, int *visitados,int *dist,int *pred,int *detonacao){
  /*Código desenvolvido na matéria de maratona de programação, modificação foi no if da linha 119*/
  for(int i = 0; i < G->n; i++) {
    visitados[i] = 0; // visitou nenhum
    dist[i] = INT_MAX; // dist infinita
    pred[i] = INT_MAX; // Caso não chega no vertice em nenhum momento
  }

  dist[0] = 0;
  PriorityQueue *q = createPriorityQueue(G->n);

  insert(q,0,0); // Inseriu o primeiro elemento na PQ
  //Identificador e peso(tempo) da aresta

  while (!isEmpty(q)){
    int tempo = 0;
    PQNode minNode = extractMin(q);
    int u = minNode.vertex;
    
    if(visitados[u] != 0) continue;
    visitados[u] = 1; // Marca o elemento como visitado

    //Ver quais os elementos que ele está ligado
    for(Aresta * a = G->V[u].arestas; a != NULL; a = a->prox){
      tempo = dist[u];
      int b = a->v;
      int w = a->tamanho;
      if((dist[b] > dist[u] + w) && ((tempo + w) < detonacao[b])){
        dist[b] = dist[u] + w;
        pred[b] = u;
        insert(q,b,dist[b]);
      }
    }
  }
  freePriorityQueue(q);
}

int* descobreCam(Grafo G, int *pred,int i){
  int* caminho = (int*)malloc(G->n * sizeof(int));

  caminho[0] = i; 
  int j=1;
  while(pred[i] != INT_MAX){
    caminho[j] = pred[i];
    i = pred[i];
    j++;
  }

  int* invertedPath = (int*)malloc((j + 1) * sizeof(int));
  for (int k = 0; k < j; k++) {
    invertedPath[k] = caminho[j - k - 1];
  }
  invertedPath[j] = INT_MAX; // Marca de término
  free(caminho);
  return invertedPath;
}

Aresta * pega_aresta1(Grafo G, int u, int v){
  //Código copiado da main.c
  for(Aresta * a = G->V[u].arestas; a != NULL; a = a->prox){
    if(a->v == v) return a;
  }
  return NULL;
}

int descobreExplo(int* caminho, int tam, Grafo G, int* detonacao) {
  //Código copiado da main.c
  int tempo = 0;  
  // printf("%d\n",caminho[0]);
  for(int i = 1; i < tam; i++){
    int u = caminho[i-1];
    int v = caminho[i];
    
    Aresta * a = pega_aresta1(G, u, v);
   
    if(a == NULL){
      printf("Sua solucao cruza uma aresta que nao existe\n");
      return(-1);
    }
    
    //acumula o tempo para viajar por aquela aresta
    tempo += a->tamanho;
    // printf("O tamanho é %d do valor %d\n",tempo, a->v);
    
    //se o vertice destino v jah explodiu quando voce chega la
    if(detonacao[v] <= tempo){
      printf("Sua solucao passa por um vertice que explodiu\n");
      return(-1);
    }
  }
  return tempo;
}


//TODO implementar essa funcao, voce pode criar outras funcoes e structs nesse arquivo
void quest_explosiva(Grafo G, int s, int * detonacao, int * baus, int *caminho, int *num_vertices_caminho){
  /*G é o grafo, s é onde começa (sempre 0), detonação é o tempo que os vetores explodem, baus é onde tem bau, caminho chega vazio, num_vertice tenho que adicionar no fim*/
  int visitados[G->n];
  int dist[G->n];
  int pred[G->n];
  dijkstra(G,visitados,dist,pred,detonacao);
  
  int tempoM = INT_MAX;
  for(int i=0;i<G->n;i++){
    if(baus[i]) {
      if(pred[i] != INT_MAX){
        int* caminhoA = descobreCam(G, pred, i);
        int tam =0;
        for (int j = 0; caminhoA[j] != INT_MAX; j++) tam++;

        int explo = descobreExplo(caminhoA,tam,G,detonacao);

        if (explo && explo < tempoM) {
          tempoM = explo;
          *num_vertices_caminho = tam;
          for (int j = 0; j < tam; j++) {
            caminho[j] = caminhoA[j];
          }
        }
        free(caminhoA);
      }
    }
  }
  free(visitados);
  free(dist);
  free(pred);
  return;
}



void imprime_grafo(Grafo G, int* detonacao, int* baus){
  printf("Grafo com %d vertice e %d arestas\n", G->n, G->m);
  for(int v = 0; v < G->n; v++){
    printf("%d (detona %d, bau %d): ", v, detonacao[v], baus[v]);
    for(Aresta * a = G->V[v].arestas; a != NULL; a = a->prox){
       printf("%d, ", a->v);
    }
    printf("\n");
  }
}

