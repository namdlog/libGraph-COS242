#include <stdio.h>
#include <iostream>
#include <list>
#include <algorithm>
#include <string>
#include <fstream>
#include <stack>
#include <queue>
#include <vector>
#include <omp.h>
#include "gLadj.h"

bool comparaComp_l (vector<int> a,vector<int> b) { return (a.size()>b.size()); }
    
gLadj::gLadj(string fName){
    
    int u;
    int v;
    int m_grauMin = 0x3f3f3f;
    int m_grauMax = 0;
    int size;
    int *grau;

    m_numArestas = 0;

    ifstream graphFile (fName);
    graphFile >> m_numVertices;

    grau = new int[m_numVertices+1]();
    vis = new bool[m_numVertices+1]();
    ni = new int[m_numVertices+1]();
    p = new int[m_numVertices+1]();
    
    lAdj = new list<int>[m_numVertices+1];

    while(graphFile >> u >> v){
        m_numArestas++;
        lAdj[u].push_back(v);
        lAdj[v].push_back(u);
        grau[u-1]++;
        grau[v-1]++;
    }
   
    m_grauMedio = ((2*m_numArestas)/m_numVertices);
    
    for(int i=1;i<=m_numVertices;i++){
        lAdj[i].sort();
        size = lAdj[i].size();
        m_grauMin = min(m_grauMin,size);
        m_grauMax = max(m_grauMax,size);
    }

    sort(grau,grau+m_numVertices);
    if(m_numVertices%2) m_grauMediano = grau[(m_numVertices/2)-1];
    else m_grauMediano = (grau[(m_numVertices/2)-1]+grau[m_numVertices/2])/2;

    ofstream infoGrafo;
    infoGrafo.open ("GrafoINFO.txt");
    infoGrafo << "Numero de vertices: " << m_numVertices << endl << "Numero de arestas: " << m_numArestas << endl << "Grau minimo: " << m_grauMin << endl << "Grau maximo: "<< m_grauMax << endl << "Grau medio: " << m_grauMedio << endl << "Grau mediano: " <<  m_grauMediano << endl;
    infoGrafo.close();
}

void gLadj::bfs_l(int s,bool printTree){

    queue<int> q;

    memset(vis,false,sizeof(bool)*(m_numVertices+1));
    
    if(printTree){
        memset(ni,0,sizeof(bool)*(m_numVertices+1));
        memset(p,0,sizeof(bool)*(m_numVertices+1));
    }
    
    q.push(s);
    vis[s] = true;
    int d = 0;
    while(!q.empty()){
        int u = q.front();
        q.pop();
        d++;
        for(int v: lAdj[u]){
            if(!vis[v]){
                if(printTree){
                    p[v] = u;
                    ni[v] = ni[u]+1;
                }
                q.push(v);
                vis[v] = true;
            }
        }   
    }
    cout << d << endl;

    if(printTree){
        ofstream bfsFile;
        bfsFile.open ("bfsFileL.txt");
        bfsFile << "VERTICE\tPAI\tNIVEL" << endl;
        for(int i=1;i<=m_numVertices;i++){
            if(i!=s && p[i] == 0 && ni[i] == 0) p[i]--,ni[i]--;
            bfsFile << i << "\t" << p[i] << "\t" << ni[i] << endl; 
        }
    }

}

void gLadj::dfs_l(int s,bool printTree){

    stack<int> st;
    
    while(!st.empty()){
        st.pop();
    }
    
    memset(vis,false,sizeof(bool)*(m_numVertices+1));

    if(printTree){
        memset(vis,0,sizeof(int)*(m_numVertices+1));
        memset(vis,0,sizeof(int)*(m_numVertices+1));

    }
    
    st.push(s);
    while(!st.empty()){
        int u = st.top();
        st.pop();
        if(!vis[u]){
            vis[u] = 1;
            for(int v: lAdj[u]){
                if(p[v] == 0 && ni[v] == 0 && v!=s && printTree){
                    p[v] = u;
                    ni[v] = ni[u]+1;
                }
                st.push(v);
            }
        } 
    }
    if(printTree){
        ofstream dfsFile;
        dfsFile.open ("dfsFileL.txt");
        dfsFile << "VERTICE\tPAI\tNIVEL" << endl;
        for(int i=1;i<=m_numVertices;i++){
            if(i!=s && p[i] == 0 && ni[i] == 0) p[i]--,ni[i]--;
            dfsFile << i << "\t" << p[i] << "\t" << ni[i] << endl; 
        }
    }
}

int gLadj::dist_l(int s, int t){

    int inf = 0x3f3f3f3f;

    queue<int> q;
    
    while(!q.empty()){
        q.pop();
    }

    memset(vis,0,sizeof(bool)*(m_numVertices+1));
    memset(ni,0,sizeof(int)*(m_numVertices+1));
    
    q.push(s);
    vis[s] = 1;
    while(!q.empty()){
        int u = q.front();
        q.pop();
        for(int v: lAdj[u]){
            if(!vis[v]){
                if(v == t) return ni[u]+1;
                ni[v] = ni[u]+1;
                q.push(v);
                vis[v] = 1;
            }
        } 
    }

    return inf;
}

int gLadj::diametro_l(){

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
            for(int v: lAdj[u]){
                if(ni[v]==-1){
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

void gLadj::componentesConexas_l(bool printComps){

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
                    for(int v: lAdj[u]){
                        st.push(v);
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
        sort(compConex,compConex+compId,comparaComp_l);
        for(int i=0;i<compId;i++){
            compoInfoFile << "Componente " << i+1 << " Tamanho : " << compConex[i].size() << ". Vertices: ";
            for(int j=0;j<compConex[i].size();j++){
                compoInfoFile << compConex[i][j] << " ";
            }
            compoInfoFile << endl;
        }
    }

    
}

