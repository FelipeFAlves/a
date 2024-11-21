#include<stdio.h>
#include<stdlib.h>
#include "grafo.h"
#include "algoritmo.h"


Aresta * pega_aresta(Grafo G, int u, int v){
  for(Aresta * a = G->V[u].arestas; a != NULL; a = a->prox){
    if(a->v == v) return a;
  }
  return NULL;
}

//Essa funcao verifica se "caminho" forma uma solucao para 
//o problema da quest_explosiva
//Devolve o custo da solucao se o estiver correta
//ou devolve um valor negativo se encontrou problema
int verifica_solucao(Grafo G, int* caminho, int num_vertices_caminho, 
                     int* detonacao, int* baus){
  if(num_vertices_caminho < 2){
    printf("Sua solucao nao tem pelo menos 2 vertices\n");
    return(-1);
  }
  if(caminho[0] != 0){
    printf("Sua solucao nao comeca no vertice 0\n");
    return(-1);
  }
  if(baus[caminho[num_vertices_caminho - 1]] != 1){
    printf("Sua solucao nao termina em um bau\n");
    return(-1);
  }

  //verifica aresta por aresta o caminho 
  int tempo = 0;  
  for(int i = 1; i < num_vertices_caminho; i++){
    int u = caminho[i-1];
    int v = caminho[i];
    
    Aresta * a = pega_aresta(G, u, v);
   
    if(a == NULL){
      printf("Sua solucao cruza uma aresta que nao existe\n");
      return(-1);
    }
    
    //acumula o tempo para viajar por aquela aresta
    tempo += a->tamanho;
    
    //se o vertice destino v jah explodiu quando voce chega la
    if(detonacao[v] <= tempo){
      printf("Sua solucao passa por um vertice que explodiu\n");
      return(-1);
    }
  }
  return tempo;
}

int main(int argc, char * argv[]){
  //Lendo o grafo
  int n, m;
  scanf("%d %d", &n, &m);
  Grafo G = criar_grafo(n);
  Grafo G_original = criar_grafo(n);

  //lendo as horas da detonacao
  int * detonacao = (int*) malloc(n * sizeof(int));
  int * detonacao_original = (int*) malloc(n * sizeof(int));
  for(int v = 0; v < n; v++){
    scanf("%d", &(detonacao[v]));
    detonacao_original[v] = detonacao[v];
  }

  //lendo se os vertices tem baus
  int * baus = (int*) malloc(n * sizeof(int));
  int * baus_original = (int*) malloc(n * sizeof(int));
  for(int v = 0; v < n; v++){
    scanf("%d", &(baus[v]));
    baus_original[v] = baus[v];
  }
  
  //lendo as arestas
  for(int i = 0; i < m; i++){
    int u, v, tamanho;
    scanf("%d %d %d", &u, &v, &tamanho);
    adiciona_aresta(G, u, v, tamanho);
    adiciona_aresta(G_original, u, v, tamanho);
  }


  int * caminho = (int*) malloc(n * sizeof(int));
  int num_vertices_caminho;
  
  //imprime_grafo(G, detonacao, baus);  
  
  //resolvendo
  quest_explosiva(G, 0, detonacao, baus, caminho, &num_vertices_caminho);
  

     
  int custo_solucao = verifica_solucao(G_original, caminho, num_vertices_caminho, detonacao_original, baus_original);
  if(custo_solucao > 0){
     /* 
       for(int i = 0; i < num_vertices_caminho; i++){
       printf("%d ", caminho[i]);
     }
     printf("\n");
     */
     printf("Sua solucao funciona e tem custo: %d\n", custo_solucao);
   }else{
     printf("Sua nao funciona");
   }
  
  free(detonacao_original);
  free(detonacao);
  free(baus_original);
  free(baus);
  free(caminho);
  //seria bom liberar a memorias dos Grafos tambem
  return 0;
}
