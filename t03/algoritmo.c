#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "grafo.h"


//TODO implementar essa funcao, voce pode criar outras funcoes e structs nesse arquivo
void quest_explosiva(Grafo G, int s, int * detonacao, int * baus, int *caminho, int *num_vertices_caminho){
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

