#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"


Grafo criar_grafo(int num_vertices){
  Grafo G = (Grafo) malloc(sizeof(tgrafo));
  G->V = (Vertice*) malloc(num_vertices * sizeof(Vertice));
  for(int i = 0; i < num_vertices; i++){
    G->V[i].arestas = NULL;
  }
  G->n = num_vertices;
  G->m = 0;
  
  return G;
}

void adiciona_aresta_em_vertice(Grafo G, int id, int destino, int tamanho){
  Aresta * a = (Aresta*) malloc(sizeof(Aresta));
  a->v = destino;
  a->tamanho = tamanho;
  a->prox = G->V[id].arestas;
  G->V[id].arestas = a;
  return;
}

void adiciona_aresta(Grafo G, int u, int v, int tamanho){
  adiciona_aresta_em_vertice(G, u, v, tamanho);
  adiciona_aresta_em_vertice(G, v, u, tamanho);
  G->m++;
  return;
}



