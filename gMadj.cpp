#include <stdio.h>
#include <iostream>
#include <chrono>
#include <string>
#include <algorithm>
#include <fstream>
#include <stack>
#include <queue>
#include "gMadj.h"

bool comparaComp_m (vector<int> a,vector<int> b) { return (a.size()>b.size()); }
    
gMadj::gMadj(string fName){

    int u;
    int v;
    int m_grauMin = 0x3f3f3f;
    int m_grauMax = 0;
    int *grau;

    ifstream graphFile (fName);
    graphFile >> m_numVertices;
    
    grau = new int[m_numVertices+1]();
    vis = new bool[m_numVertices+1]();
    ni = new int[m_numVertices+1]();
    p = new int[m_numVertices+1]();

    mAdj = new bool *[m_numVertices+1];
    
    for(int i=0;i<=m_numVertices;i++){
        mAdj[i] = new bool[m_numVertices+1];
    }

    for(int i=0;i<=m_numVertices;i++){
        for(int j=0;j<m_numVertices+1;j++){
            mAdj[i][j] = false;
        }
    }
    
    m_numArestas = 0;
    
    while(graphFile >> u >> v){
        m_numArestas++;
        grau[u]++;
        grau[v]++;
        m_grauMin = min(m_grauMin,min(grau[u],grau[v]));
        m_grauMax = max(m_grauMax,max(grau[u],grau[v]));
        mAdj[u][v] = true;
        mAdj[v][u] = true;
    }
   
    m_grauMedio = ((2*m_numArestas)/m_numVertices);

    sort(grau,grau+m_numVertices);
    if(m_numVertices%2) m_grauMediano = grau[(m_numVertices/2)-1];
    else m_grauMediano = (grau[(m_numVertices/2)-1]+grau[m_numVertices/2])/2;

    ofstream infoGrafo;
    infoGrafo.open ("GrafoINFO.txt");
    infoGrafo << "Numero de vertices: " << m_numVertices << endl << "Numero de arestas: " << m_numArestas << endl << "Grau minimo: " << m_grauMin << endl << "Grau maximo: "<< m_grauMax << endl << "Grau medio: " << m_grauMedio << endl << "Grau mediano: " <<  m_grauMediano << endl;
    infoGrafo.close();
}

void gMadj::bfs_m(int s,bool printTree){

    queue<int> q;

    while(!q.empty()){
        q.pop();
    }
    auto start = chrono::system_clock::now();
    #pragma omp for
    for(int k=1;k<=10;k++){
    memset(vis,false,sizeof(bool)*(m_numVertices+1));
    /*if(printTree){
        memset(p,0,sizeof(int)*(m_numVertices+1));
        memset(ni,0,sizeof(int)*(m_numVertices+1));
    }
    */
    q.push(k);
    vis[k] = 1;
    while(!q.empty()){
        int u = q.front();
        q.pop();
        for(int v = 1;v<=m_numVertices+1;v++){
            if(!vis[v] && mAdj[u][v]){
                if(printTree){
                    p[v] = u;
                    ni[v] = ni[u]+1;
                }
                q.push(v);
                vis[v] = 1;
            }
        } 
    }
    }
    auto end = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = end-start;
    time_t end_time = std::chrono::system_clock::to_time_t(end);
    cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
    if(printTree){
        ofstream bfsFile;
        bfsFile.open ("bfsFileM.txt");
        bfsFile << "VERTICE\tPAI\tNIVEL" << endl;
        for(int i=1;i<=m_numVertices+2;i++){
            if(i!=s && p[i] == 0 && ni[i] == 0) p[i]--,ni[i]--;
            bfsFile << i << "\t" << p[i] << "\t" << ni[i] << endl; 
        }
    }
}

void gMadj::dfs_m(int s,bool printTree){
    
    stack<int> st;

    while(!st.empty()){
        st.pop();
    }

    memset(vis,false,sizeof(bool)*(m_numVertices+1));
    if(printTree){
        memset(p,0,sizeof(int)*(m_numVertices+1));
        memset(ni,0,sizeof(int)*(m_numVertices+1));
    }    

    st.push(s);

    while(!st.empty()){
        int u = st.top();
        st.pop();
        if(!vis[u]){
            vis[u] = 1;
            for(int v = 0;v<=m_numVertices+2;v++){
                if(mAdj[u][v]){
                    if(!p[v]  && !ni[v] && v!= s && printTree){
                        p[v] = u;
                        ni[v] = ni[u]+1;
                    }
                    st.push(v);
                }
            }
        } 
    }
    
    if(printTree){
        ofstream dfsFile;
        dfsFile.open ("dfsFileM.txt");
        dfsFile << "VERTICE\tPAI\tNIVEL" << endl;
        for(int i=1;i<=m_numVertices;i++){
            if(i!=s && p[i] == 0 && ni[i] == 0) p[i]--,ni[i]--;
            dfsFile << i << "\t" << p[i] << "\t" << ni[i] << endl; 
        }
    }
}

int gMadj::dist_m(int s, int t){

    int inf = 0x3f3f3f3f;

    memset(p,0,sizeof(int)*(m_numVertices+1));
    memset(ni,0,sizeof(int)*(m_numVertices+1));
    
    queue<int> q;

    q.push(s);
    vis[s] = 1;
    while(!q.empty()){
        int u = q.front();
        q.pop();
        for(int v = 1;v<=m_numVertices;v++){
            if(!vis[v] && mAdj[u][v]){
                if(v == t) return ni[u]+1;
                ni[v] = ni[u]+1;
                q.push(v);
                vis[v] = 1;
            }
        } 
    }

    return inf;
}

int gMadj::diametro_m(){

    int mx = 0;
    m_diametro = 0;

    queue<int> q;
    
    while(!q.empty()){
        q.pop();
    }

    for(int i=1;i<=m_numVertices;i++){
        mx = 0;
        for(int j=1;j<=m_numVertices;j++){
            ni[j]=-1;
        }
        q.push(i);
        ni[i] = 0;
        while(!q.empty()){
            int u = q.front();
            q.pop();
            for(int v = 1;v<=m_numVertices+1;v++){
                if(ni[v]==-1 && mAdj[u][v]){
                    ni[v] = ni[u]+1;
                    mx = max(mx,ni[v]);
                    q.push(v);
                }
            } 
        }
        m_diametro = max(m_diametro,mx);
    }

    return m_diametro;
}
void gMadj::componentesConexas_m(bool printComps){

    memset(vis,0,sizeof(bool)*m_numVertices);

    int compId = 0;

    compConex = new vector<int>[m_numVertices];

    for(int i=1;i<=m_numVertices;i++){
        if(!vis[i]){
            stack<int> st;
            st.push(i);
            while(!st.empty()){
                int u = st.top();
                st.pop();
                if(!vis[u]){
                    vis[u] = 1;
                    compConex[compId].push_back(u);
                    for(int v=1;v<=m_numVertices;v++){
                        if(mAdj[u][v]){
                            st.push(v);
                        }
                    }
                } 
            }
            compId++;
        }
    }
    
    
    if(printComps){
        ofstream compoInfoFile;
        compoInfoFile.open ("compoInfoFile.txt");
        compoInfoFile << "Numero de componentes conexas: " << compId << endl;
        sort(compConex,compConex+compId,comparaComp_m);
        for(int i=0;i<compId;i++){
            compoInfoFile << "Componente " << i+1 << " Tamanho : " << compConex[i].size() << ". Vertices: ";
            for(int j=0;j<compConex[i].size();j++){
                compoInfoFile << compConex[i][j] << " ";
            }
            compoInfoFile << endl;
        }
    }
   
}
