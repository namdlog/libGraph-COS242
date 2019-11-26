#include <stdio.h>
#include <string.h>
#include <stack>
#include <queue>
#include <omp.h>
#include <chrono>

using namespace std;

class gMadj{

    public:
        int m_numVertices;                  // Número de vértices do grafo
        int m_numArestas;                   // Número de Arestas
        int m_diametro;                     // Diâmetro do Grafo
        int m_grauMin;                      // Grau Mínimo
        int m_grauMax;                      // Grau Máximo
        int m_grauMedio;                    // Grau Médio 
        int m_grauMediano;                  // Grau Mediano
        int m_digraph;                      // Flag que diz se o grafo é direcionado
        bool is_bipartie;                   // Flag que diz se o grafo é bipartido ou não

        bool *vis;                          // Ponteiro para Array de visitados
        int *visConex;                      // Ponteiro para Array de visitados das componentes conexas
        int *p;                             // Ponteiro para Array de Pais
        int *ni;                            // Ponteiro para Array de Nível
        int *grau;                          // ponteiro para vetor que armazena o grau
        double *d;                          // Ponteiro para Array de distâncias calculadas pelo algoritmo de Dijkstra
        int *pd;                            // Ponteiro para Array que armazena o pai dos vértices durante o algoritmo de Dijkstra
        bool fd;                            // Flag de Dijkstra
        bool td;                            // Teste de Dijkstra
        double *custo;                      // Ponteiro para Array de custo do algoritmo de Prim para calcular a MST 
        double *ex;                         // Ponteiro para Array que armazena a excentricidade de um vértice
        int *pmst;                          // Ponteiro para Array que armazena a árvore do MST
        int *b;                             // Ponteiro para Array que armazena os matching do Hopcroft-Karp
        int *dist;                          // Distância do algoritmo de HopCroft-Karp
        double *dbf;                        // Array para a distância de Bellmanford

        double **mAdj;                      // Matriz de Adjacência do Grafo
        vector<int> *compConex;             // Array de componentes conexas
        gMadj(string fName);                // Construtor do Grafo em Matriz
        vector<int> *graphBipartite_A;
        vector<int> *graphBipartite_B;

        int diametro_m();                   // Função Diâmetro
        int dist_bfs(int s, int t);         // Função que calcula a Distância por BFS em grafo sem pesos
        void dist_m(int s, int t);           // Função Distância
        void bfs_m(int u,bool printTree);   // Busca em Largura
        void dfs_m(int u,bool printTree);   // Busca em Profundidade
        void componentesConexas_m();        // Componentes Conexas em Matriz
        void Dijkstra_m(int u);             // Algoritmo de Dijkstra com source no vértice u
        void mst_m(int u);                  // Algoritmo para gerar a MST do grafo
        double exc_m(int u);                // Calcula a Excentricidade de um vértice
        int hk();                           // Algoritmo de HopCroft-Karp que Calcula o Matching Máximo em Grafo Bipartido.
        bool dfs( int n );                  // DFS do HopCroft-Karp
        bool bfs();                         // BFS do HopCroft-Karp
        void BellmanFord(int s);            // Algoritmo de BellmanFord que calcula as distâncias entre um vértice a todos os outros
        void BellmanMatriz();               // Algoritmo de BellmanFord de todos os vértices para todos

};
