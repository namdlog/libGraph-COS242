#include <stdio.h>
#include <string.h>
#include <stack>
#include <queue>
#include <omp.h>
#include <chrono>

using namespace std;

class gMadj{

    public:
        bool *vis;
        int *p;
        int *ni;
        int m_numVertices;
        int m_numArestas;
        int m_diametro;
        int m_grauMin;
        int m_grauMax;
        int m_grauMedio;
        int m_grauMediano;
        gMadj(string fName);
        bool **mAdj;
        vector<int> *compConex;
        void bfs_m(int u,bool printTree);
        void dfs_m(int u,bool printTree);
        void componentesConexas_m(bool printComps);
        int diametro_m();
        int dist_m(int s, int t);
};
