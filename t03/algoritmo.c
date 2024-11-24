#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "grafo.h"
#include <stdbool.h>

/*
Pai = (index-1)/2
Filho esquerdo = 2*index + 1
Filho direito = 2*index + 2
*/

typedef struct {
  int id;
  int prioridade;
} No;

typedef struct {
  No *nos;
  int tamanho;   // Número de elementos na fila
  int capacidade; // Capacidade máxima da fila
} PriorityQueue;

// Funções auxiliares
int indexFilhoEsquerda(int indexPai) { return (indexPai * 2) + 1; }
int indexFilhoDireita(int indexPai) { return (indexPai * 2) + 2; }
int indexPai(int index) { return (index - 1) / 2; }
bool existeFilhoEsquerda(int index, PriorityQueue *q) { return indexFilhoEsquerda(index) < q->tamanho; }
bool existeFilhoDireita(int index, PriorityQueue *q) { return indexFilhoDireita(index) < q->tamanho; }
bool existePai(int index) { return indexPai(index) >= 0; }
No filhoEsquerda(int index, PriorityQueue *q) { return q->nos[indexFilhoEsquerda(index)]; }
No filhoDireita(int index, PriorityQueue *q) { return q->nos[indexFilhoDireita(index)]; }
No pai(int index, PriorityQueue *q) { return q->nos[indexPai(index)]; }

void swap(int index1, int index2, PriorityQueue *q) {
  No aux = q->nos[index1];
  q->nos[index1] = q->nos[index2];
  q->nos[index2] = aux;
}

void heapifyUp(PriorityQueue *q) {
  int index = q->tamanho - 1;
  while (existePai(index) && pai(index, q).prioridade > q->nos[index].prioridade) {
    swap(indexPai(index), index, q);
    index = indexPai(index);
  }
}

void heapifyDown(PriorityQueue *q) {
  int index = 0;
  while (existeFilhoEsquerda(index, q)) {
    int menorFilho = indexFilhoEsquerda(index);
    if (existeFilhoDireita(index, q) && filhoDireita(index, q).prioridade < filhoEsquerda(index, q).prioridade) {
      menorFilho = indexFilhoDireita(index);
    }
    if (q->nos[index].prioridade < q->nos[menorFilho].prioridade) break;
    swap(index, menorFilho, q);
    index = menorFilho;
  }
}

No poll(PriorityQueue *q) {
  No menorItem = q->nos[0];
  q->nos[0] = q->nos[q->tamanho - 1];
  q->tamanho--;
  heapifyDown(q);
  return menorItem;
}

void add(No item, PriorityQueue *q) {
  q->nos[q->tamanho] = item;
  q->tamanho++;
  heapifyUp(q);
}

void freePriorityQueue(PriorityQueue *q) {
  free(q->nos);
  free(q);
}

PriorityQueue* criaFila(int capacity) {
  PriorityQueue *pq = (PriorityQueue *)malloc(sizeof(PriorityQueue));
  pq->nos = (No *)malloc(sizeof(No) * capacity);
  pq->tamanho = 0;
  pq->capacidade = capacity;
  return pq;
}

void dijkstra(Grafo G, int *visitados, int *dist, int *pred, int *detonacao) {
  for (int i = 0; i < G->n; i++) {
    visitados[i] = 0;
    dist[i] = INT_MAX;
    pred[i] = INT_MAX;
  }

  dist[0] = 0;
  PriorityQueue *q = criaFila(G->n);

  No novoNo;
  novoNo.id = 0;
  novoNo.prioridade = 0;

  add(novoNo, q);

  while (q->tamanho) {
    No minNode = poll(q);
    int u = minNode.id;

    if (visitados[u]) continue;
    visitados[u] = 1;

    for (Aresta *a = G->V[u].arestas; a != NULL; a = a->prox) {
      int b = a->v;
      int w = a->tamanho;

      if ((dist[b] > dist[u] + w) && (dist[u] + w < detonacao[b])) {
        dist[b] = dist[u] + w;
        pred[b] = u;

        No novoNo;
        novoNo.id = b;
        novoNo.prioridade = dist[b];
        add(novoNo, q);
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

