
#include <list>
#include <string.h>
#include <omp.h>
#include <stack>
#include <queue>
#include <chrono>
#include <vector>


using namespace std;

class gLadj{

    public:
        int m_numVertices;
        int m_numArestas;
        int m_diametro;
        int m_grauMin;
        int m_grauMax;
        int m_grauMedio;
        int m_grauMediano;
        bool *vis;
        int *p;
        int *ni;
        list<int> *lAdj;
        vector<int> *compConex;
        gLadj(string fName);
        int diametro_l();
        int dist_l(int s, int t);
        void bfs_l(int u,bool printTree);
        void dfs_l(int u,bool printTree);
        void componentesConexas_l(bool printComps);

};
