//Tres registros que serao uteis, Arco, Vertice e Grafo
typedef struct aresta{
	int v; // VALUE
	int tamanho;
	struct aresta * prox;
}Aresta;

typedef struct vertice{
	Aresta * arestas; 
}Vertice;

typedef struct tgrafo{
	int n; //numero de vertices
	int m; //numero de arestas
	Vertice * V;
}tgrafo;

typedef tgrafo * Grafo;

//funcao que cria um grafo
//recebe o numero de vertices e o grafo criado
//nao tem nenhuma aresta
Grafo criar_grafo(int num_vertices);

//funcao que adiciona a aresta nao direcionada {u, v} no grafo G
void adiciona_aresta(Grafo G, int u, int v, int tamanho);
