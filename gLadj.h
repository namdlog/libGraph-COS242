
#include <list>
#include <string.h>
#include <omp.h>
#include <stack>
#include <queue>
#include <vector>


using namespace std;

class gLadj{

    public:
        int m_numVertices;                  // Número de vértices do grafo
        int m_numArestas;                   // Número de Arestas
        int m_diametro;                     // Diâmetro do Grafo
        int m_grauMin;                      // Grau Mínimo
        int m_grauMax;                      // Grau Máximo
        int m_grauMedio;                    // Grau Médio 
        int m_grauMediano;                  // Grau Mediano

        bool *vis;                          // Ponteiro para Array de visitados
        int *visConex;                      // Ponteiro para Array de visitados das componentes conexas
        int *p;                             // Ponteiro para Array de Pais
        int *ni;                            // Ponteiro para Array de Nível
        int *grau;                          // ponteiro para vetor que armazena o grau

        list<int> *lAdj;                    // Lista de Adjacência do Grafo
        vector<int> *compConex;             // Array de componentes conexas
        gLadj(string fName);                // Construtor do Grafo em Lista
        int diametro_l();                   // Função Diâmetro
        int dist_l(int s, int t);           // Função Distância
        void bfs_l(int u,bool printTree);   // Busca em Largura
        void dfs_l(int u,bool printTree);   // Busca em Profundidade
        void componentesConexas_l();        // Componentes Conexas em Lista

};
