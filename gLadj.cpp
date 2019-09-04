#include <stdio.h>
#include <iostream>
#include <list>
#include <stack>
#include <queue>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <omp.h>
#include "gLadj.h"

// Função de comparar tamanho de vetores para ordenar as componentes conexas
bool comparaComp_l (vector<int> a,vector<int> b) { return (a.size()>b.size()); }
    
// Construtor do grafo em lista de adjacência
gLadj::gLadj(string fName){
    
    // Variáveis utlizadas durante o preenchimento do grafo
    int u;                    // Vértice genérico u
    int v;                    // Vértice genérico v
    int m_grauMin = 0x3f3f3f; // Grau mínimo do grafo
    int m_grauMax = 0;        // Grau máximo do grafo
    int size;                 // Grau de um vértice

    m_numArestas = 0; // Número de arestas é inicializado com 0

    // Leitura do arquivo passado no parâmetro do construtor
    ifstream graphFile (fName);
    graphFile >> m_numVertices; // Lê na primeira linha do arquivo o número de vértices do grafo

    grau = new int[m_numVertices+1](); // Array de grau de um vértice
    vis = new bool[m_numVertices+1](); // Array de visitado para algoritmos busca
    ni = new int[m_numVertices+1]();   // Array de nível para árvore de busca
    p = new int[m_numVertices+1]();    // Array de pai para árvore de busca
    
    // Estrutura do grafo em sí. array de listas de adjacência
    lAdj = new list<int>[m_numVertices+1]();

    // Leitura e armazenamento dos finais das arestas
    while(graphFile >> u >> v){
        m_numArestas++;
        lAdj[u].push_back(v);
        lAdj[v].push_back(u);
        grau[u-1]++;
        grau[v-1]++;
    }
   
    // Cálculo do grau médio
    m_grauMedio = ((2*m_numArestas)/m_numVertices);
    
    // Ordenação da lista de adjacência e cálculo do grau mínimo e máximo
    for(int i=1;i<=m_numVertices;i++){
        lAdj[i].sort();
        size = lAdj[i].size();
        m_grauMin = min(m_grauMin,size);
        m_grauMax = max(m_grauMax,size);
    }

    // Ordena grau para obter mediana
    sort(grau,grau+m_numVertices);
    if(m_numVertices%2) m_grauMediano = grau[(m_numVertices/2)-1];
    else m_grauMediano = (grau[(m_numVertices/2)-1]+grau[m_numVertices/2])/2;

    // Imprime arquivo contendo informações do grafo
    ofstream infoGrafo;
    infoGrafo.open ("GrafoINFO.txt");
    infoGrafo << "Numero de vertices: " << m_numVertices << endl << "Numero de arestas: " << m_numArestas << endl << "Grau minimo: " << m_grauMin << endl << "Grau maximo: "<< m_grauMax << endl << "Grau medio: " << m_grauMedio << endl << "Grau mediano: " <<  m_grauMediano << endl;
    infoGrafo.close();
}

// (BFS) Breadth-First Search na lista
void gLadj::bfs_l(int s,bool printTree){

    // Fila de busca
    queue<int> q;

    // Limpa fila caso esteja com algo antes   
    while(!q.empty()){
        q.pop();
    }
    
    // Limpa o vetor de visitados antes de começar o algoritmo
    memset(vis,false,sizeof(bool)*(m_numVertices+1));
    
    // Caso o usuário queira imprimir o arquivo da árvore de busca, esse bloco limpa os arrays de pai e nível
    if(printTree){
        memset(ni,0,sizeof(bool)*(m_numVertices+1));
        memset(p,0,sizeof(bool)*(m_numVertices+1));
    }
    
    // BFS
    q.push(s);
    vis[s] = true;
    while(!q.empty()){
        int u = q.front();
        q.pop();
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

    // Bloco executado caso o usuário queira imprimir o arquivo da árvore de busca
    if(printTree){
        ofstream bfsFile;
        bfsFile.open ("bfsFileL.txt");
        bfsFile << "VERTICE\tPAI\tNIVEL" << endl;
        // Para cada vértice exibe o pai e o nível
        for(int i=1;i<=m_numVertices;i++){
            // Caso o vértice não tenha sido visitado pela bfs ele possui pai e nivel -1
            if(i!=s && p[i] == 0 && ni[i] == 0) p[i]--,ni[i]--;
            bfsFile << i << "\t" << p[i] << "\t" << ni[i] << endl; 
        }
    }

}

// (DFS) Depth-First Search na lista
void gLadj::dfs_l(int s,bool printTree){

    // Pilha
    stack<int> st;
    
    // Limpa pilha caso esteja com algo antes
    while(!st.empty()){
        st.pop();
    }
    
    // Limpa o vetor de visitados antes de começar o algoritmo
    memset(vis,false,sizeof(bool)*(m_numVertices+1));

    // Caso o usuário queira imprimir o arquivo da árvore de busca, esse bloco limpa os arrays de pai e nível
    if(printTree){
        memset(ni,0,sizeof(bool)*(m_numVertices+1));
        memset(p,0,sizeof(bool)*(m_numVertices+1));
    }

    // DFS
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

    // Bloco executado caso o usuário queira imprimir o arquivo da árvore de busca
    if(printTree){
        ofstream dfsFile;
        dfsFile.open ("dfsFileL.txt");
        // Para cada vértice exibe o pai e o nível
        dfsFile << "VERTICE\tPAI\tNIVEL" << endl;
        for(int i=1;i<=m_numVertices;i++){
            // Caso o vértice não tenha sido visitado pela bfs ele possui pai e nivel -1s
            if(i!=s && p[i] == 0 && ni[i] == 0) p[i]--,ni[i]--;
            dfsFile << i << "\t" << p[i] << "\t" << ni[i] << endl; 
        }
    }
}

// Distância entre dois vértices no grafo
int gLadj::dist_l(int s, int t){

    // Infinito caso eles estejam em componentes conexas distintas
    int inf = 0x3f3f3f3f;

    // Fila
    queue<int> q;
    
    // Limpa a fila
    while(!q.empty()){
        q.pop();
    }

    // Limps os vetores utilizados no cálculo da distância
    memset(vis,0,sizeof(bool)*(m_numVertices+1));
    memset(ni,0,sizeof(int)*(m_numVertices+1));
    
    // BFS
    q.push(s);
    vis[s] = 1;
    while(!q.empty()){
        int u = q.front();
        q.pop();
        for(int v: lAdj[u]){
            if(!vis[v]){
                // Caso o vértice desejado seja alcançado retorna o nível que é igual a distância
                if(v == t) return ni[u]+1;
                ni[v] = ni[u]+1;
                q.push(v);
                vis[v] = 1;
            }
        } 
    }

    // Caso so algoritmo não tenha retornado nada, significa que eles estão em componentes desconexas, logo sua distância é infinita
    return inf;
}

// Diâmetro do Grafo
int gLadj::diametro_l(){

    int mx = 0;     // Variável máximo
    m_diametro = 0; // Variável diâmetro

    // Fila
    queue<int> q;
    
    // Limpa a fila
    while(!q.empty()){
        q.pop();
    }

    // Para cada vértice eu vejo o maior nível que eu consigo alcançar através de uma BFS em cada vértice.
    for(int i=1;i<=m_numVertices;i++){
        mx = 0;
        // O array de nível vira o vetor de visitados e no final o maior valor de nível é um candidato a diâmetro
        for(int j=1;j<=m_numVertices;j++){
            ni[j]=-1;
        }
        // BFS
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
        m_diametro = max(m_diametro,mx); // diâmetro recebe o máximo entre o diâmetro e mx (máximo nível uma bfs)
    }

    return m_diametro;


}

// Componentes conexas
void gLadj::componentesConexas_l(){

    // Array de visitados para componente conexa
    visConex = new int[m_numVertices+1]();

    // Limpa o vetor de visitados
    memset(visConex,0,sizeof(int)*m_numVertices);

    // Número de componente conexas
    int compId = 0;

    // Fila
    queue<int> q;

    // BFS para cada vértice, armazenando seu id da componente conexa
    for(int i=1;i<=m_numVertices;i++){
        if(!visConex[i]){
            compId++;
            q.push(i);
            visConex[i] = compId;
            while(!q.empty()){
                int u = q.front();
                q.pop();
                for(int v: lAdj[u]){
                    if(!visConex[v]){
                        q.push(v);
                        visConex[v] = compId;
                    }
                }   
            }
        }
    }
    
    // Array de componentes conexas
    compConex = new vector<int>[compId+1];

    // Para cada vértice, adiciono ele no array de sua componente
    for(int i=1;i<=m_numVertices;i++){
        if(visConex[i])  compConex[visConex[i]-1].push_back(i);
    }
    
    // Exibe arquivo com as componentes ordenadas, seu tamanho e seus vértices
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

