#include <stdio.h>
#include <iostream>
#include <list>
#include <stack>
#include <chrono>
#include <queue>
#include <vector>
#include <set>
#include <algorithm>
#include <stdio.h> 
#include <string.h>
#include <string>
#include <fstream>
#include <functional>
#include <omp.h>
#include "gMadj.h"

// Função de comparar tamanho de vetores para ordenar as componentes conexas
bool comparaComp_m (vector<int> a,vector<int> b) { return (a.size()>b.size()); }

// Constante que representa um número muito grande
const double INF = 0x3f3f3f3f;

// Construtor do grafo em Matriz de adjacência    
gMadj::gMadj(string fName){

    // Variáveis utlizadas durante o preenchimento do grafo
    int u;                       // Vértice genérico u
    int v;                       // Vértice genérico v
    double w;                    // Peso da aresta entre u e v
    int m_grauMin = 0x3f3f3f;    // Grau mínimo do grafo
    int m_grauMax = 0;           // Grau máximo do grafo
    int size;                    // Grau de um vértice

    m_numArestas = 0; // Número de arestas é inicializado com 0
    
    // Leitura do arquivo passado no parâmetro do construtor
    int h=0,r=0,td=0,tdg=0;
    string s;

    ifstream graphFileTesteUm (fName);
    
        getline(graphFileTesteUm,s);
        for(int i=0;i<s.size();i++){
            if(s[i] == ' ') h++;
        }
        if(h){
            tdg = 1;
        }
    graphFileTesteUm.close();
    
    ifstream graphFileTeste (fName);
    
        getline(graphFileTeste,s);
        getline(graphFileTeste,s);
        for(int i=0;i<s.size();i++){
            if(s[i] == ' ') 
                r++;
        }
        if(r == 2) 
            td=1;
        else 
            td=0;

    graphFileTeste.close();

    ifstream graphFile (fName);
    graphFile >> m_numVertices; // Lê na primeira linha do arquivo o número de vértices do grafo
    if(tdg) graphFile >> m_digraph;
    
    grau = new int[m_numVertices+1]();     // Array de grau de um vértice
    vis = new bool[m_numVertices+1]();     // Array de visitado para algoritmos busca
    ni = new int[m_numVertices+1]();       // Array de nível para árvore de busca
    p = new int[m_numVertices+1]();        // Array de pai para árvore de busca
    d = new double[m_numVertices+1]();     // Array de distâncias calculadas pelo algoritmo de Dijksta
    custo = new double[m_numVertices+1](); // Array de custo da MST
    pd = new int[m_numVertices+1]();       // Array de pai de Dijksta
    ex = new double[m_numVertices+1]();    // Array da excentricidade de um vértice
    pmst = new int [m_numVertices+1]();    // Array com a árvore geradora mínimas
    b = new int[m_numVertices+1];          // Array que armazena o matching de cada vértice
    dist = new int[m_numVertices+1]();     // Array que armazena distância do HopCroft-Karp
    dbf = new double[m_numVertices+1]();   // Array que armazena as distâncias do Bellmanford
    graphBipartite_A = new vector<int>;    // Vetor que armazena os vértices do grupo A do Grafo bipartido
    graphBipartite_B = new vector<int>;    // Vetor que armazena os vértices do grupo B do Grafo bipartido
    
    // Estrutura do grafo em sí. Array de arrays. Matriz de Adjacência
    mAdj = new double *[m_numVertices+1];
    
    // Cada posição do array aloca um array que representa uma linha na matriz
    for(int i=0;i<=m_numVertices;i++){
        mAdj[i] = new double[m_numVertices+1];
    }

    // Deixa tudo como 0.0
    for(int i=0;i<=m_numVertices;i++){
        for(int j=0;j<m_numVertices+1;j++){
            mAdj[i][j] = 0.0;
        }
    }
    
    //Define número de arestas inicialmente como zeros
    m_numArestas = 0;

    // Leitura e armazenamento dos finais das arestas
    
    // Caso o grafo seja um grafo direcionado ou não, as leituras serão diferentes
    if(m_digraph){
        if(td){
            while(graphFile >> u >> v >> w){
                m_numArestas++;
                grau[u]++;
                grau[v]++;
                m_grauMin = min(m_grauMin,min(grau[u],grau[v]));
                m_grauMax = max(m_grauMax,max(grau[u],grau[v]));
                mAdj[u][v] = w;
                if(w < 0.0){
                    fd = 1;
                }
            }
        }else{
            fd = 1;
            while(graphFile >> u >> v){
                m_numArestas++;
                grau[u]++;
                grau[v]++;
                m_grauMin = min(m_grauMin,min(grau[u],grau[v]));
                m_grauMax = max(m_grauMax,max(grau[u],grau[v]));
                mAdj[u][v] = 1;
            }
        }
    }else{
        if(td){
            while(graphFile >> u >> v >> w){
                m_numArestas++;
                grau[u]++;
                grau[v]++;
                m_grauMin = min(m_grauMin,min(grau[u],grau[v]));
                m_grauMax = max(m_grauMax,max(grau[u],grau[v]));
                mAdj[u][v] = w;
                mAdj[v][u] = w;
                if(w < 0.0){
                    fd = 1;
                }
            }
        }else{
            fd = 1;
            while(graphFile >> u >> v){
                m_numArestas++;
                grau[u]++;
                grau[v]++;
                m_grauMin = min(m_grauMin,min(grau[u],grau[v]));
                m_grauMax = max(m_grauMax,max(grau[u],grau[v]));
                mAdj[u][v] = 1;
                mAdj[v][u] = 1;
            }
        }
    }
    
    // Cálculo do grau médio
    m_grauMedio = ((2*m_numArestas)/m_numVertices);

    // Ordena dos graus para obter a mediana
    sort(grau,grau+m_numVertices);
    if(m_numVertices%2) m_grauMediano = grau[(m_numVertices/2)-1];
    else m_grauMediano = (grau[(m_numVertices/2)-1]+grau[m_numVertices/2])/2;

    // Imprime arquivo contendo informações do grafos
    ofstream infoGrafo;
    infoGrafo.open ("GrafoINFO.txt");
    infoGrafo << "Numero de vertices: " << m_numVertices << endl << "Numero de arestas: " << m_numArestas << endl << "Grau minimo: " << m_grauMin << endl << "Grau maximo: "<< m_grauMax << endl << "Grau medio: " << m_grauMedio << endl << "Grau mediano: " <<  m_grauMediano << endl;
    infoGrafo.close();
}

// (BFS) Breadth-First Search na matriz
void gMadj::bfs_m(int s,bool printTree){

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
        memset(p,0,sizeof(int)*(m_numVertices+1));
        memset(ni,0,sizeof(int)*(m_numVertices+1));
    }

    // BFS que colore em 2 cores, 0 e 1.
    q.push(s);
    vis[s] = 0;
    while(!q.empty()){
        int u = q.front();
        int c = vis[u];
        q.pop();
        for(int v =1;v<=m_numVertices;v++){
            if(mAdj[u][v]){
                if(vis[v] == -1){
                    // Caso a cor do vértice analisado na BFS, os seus filhos tem que ter a cor oposta.
                    vis[v] = vis[u]^1;
                    p[v] = u;
                    ni[v] = ni[u]+1;
                    q.push(v);
                }else{
                    // Caso a cor do seu filho seja igual ao seu, o grafo não é bipartido
                    is_bipartie &= vis[u] != vis[v];
                    if(is_bipartie == 0){
                        break;
                    }
                }
            }
        }   
    }
    if(is_bipartie){
        // Bloco executado caso o usuário queira imprimir o arquivo da árvore de busca
        if(printTree){
            ofstream bfsFile;
            bfsFile.open ("bfsFileL.txt");
            bfsFile << "O grafo é bipartido." << endl;
            bfsFile << "VERTICE\tPAI\tNIVEL" << endl;
            // Para cada vértice exibe o pai e o nível
            for(int i=1;i<=m_numVertices;i++){
                // Caso o vértice não tenha sido visitado pela bfs ele possui pai e nivel -1
                if(i!=s && p[i] == 0 && ni[i] == 0) p[i]--,ni[i]--;
                bfsFile << i << "\t" << p[i] << "\t" << ni[i] << "\t" << vis[i] << endl; 
                if(vis[i] == 0){
                    graphBipartite_A->push_back(i);
                }
                if(vis[i] == 1){
                    graphBipartite_B->push_back(i);
                }
            }
            
            bfsFile.close();
        }
    }else{
        printf("O grafo não é bipartido.");
    }
    
    // Bloco executado caso o usuário queira imprimir o arquivo da árvore de busca
    if(printTree){
        ofstream bfsFile;
        bfsFile.open ("bfsFileM.txt");
        bfsFile << "VERTICE\tPAI\tNIVEL" << endl;
        // Para cada vértice exibe o pai e o nível
        for(int i=1;i<=m_numVertices+2;i++){
            // Caso o vértice não tenha sido visitado pela bfs ele possui pai e nivel -1
            if(i!=s && p[i] == 0 && ni[i] == 0) p[i]--,ni[i]--;
            bfsFile << i << "\t" << p[i] << "\t" << ni[i] << endl; 
        }
        bfsFile.close();
    }
}

// (DFS) Depth-First Search na lista
void gMadj::dfs_m(int s,bool printTree){
    
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
        memset(p,0,sizeof(int)*(m_numVertices+1));
        memset(ni,0,sizeof(int)*(m_numVertices+1));
    }    

    // DFS
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
    
    // Bloco executado caso o usuário queira imprimir o arquivo da árvore de busca
    if(printTree){
        ofstream dfsFile;
        dfsFile.open ("dfsFileM.txt");
        // Para cada vértice exibe o pai e o nível
        dfsFile << "VERTICE\tPAI\tNIVEL" << endl;
        for(int i=1;i<=m_numVertices;i++){
            // Caso o vértice não tenha sido visitado pela bfs ele possui pai e nivel -1
            if(i!=s && p[i] == 0 && ni[i] == 0) p[i]--,ni[i]--;
            dfsFile << i << "\t" << p[i] << "\t" << ni[i] << endl; 
        }
        dfsFile.close();
    }
}

// Distância entre dois vértices no grafo
int gMadj::dist_bfs(int s, int t){

    // Infinito caso eles estejam em componentes conexas distintas
    int inf = 0x3f3f3f3f;

    // Limps os vetores utilizados no cálculo da distâncias
    memset(p,0,sizeof(int)*(m_numVertices+1));
    memset(ni,0,sizeof(int)*(m_numVertices+1));
    
    // Fila
    queue<int> q;

    // Limpa a fila
    while(!q.empty()){
        q.pop();
    }

    // BFS
    q.push(s);
    vis[s] = 1;
    while(!q.empty()){
        int u = q.front();
        q.pop();
        for(int v = 1;v<=m_numVertices;v++){
            if(!vis[v] && mAdj[u][v]){
                // Caso o vértice desejado seja alcançado retorna o nível que é igual a distância
                if(v == t) return ni[u]+1;
                ni[v] = ni[u]+1;
                q.push(v);
                vis[v] = 1;
            }
        } 
    }

    // Caso so algoritmo não tenha retornado nada ainda, significa que eles estão em componentes desconexas, logo sua distância é infinita
    return inf;
}

// Diâmetro do Grafo
int gMadj::diametro_m(){

    int mx = 0;       // Variável máximo
    m_diametro = 0;   // Variável diâmetro

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
            for(int v = 1;v<=m_numVertices+1;v++){
                if(ni[v]==-1 && mAdj[u][v]){
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
void gMadj::componentesConexas_m(){

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
                for(int v = 0;v<=m_numVertices;v++){
                    if(!visConex[v] && mAdj[u][v]){
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
    sort(compConex,compConex+compId,comparaComp_m);
    for(int i=0;i<compId;i++){
        compoInfoFile << "Componente " << i+1 << " Tamanho : " << compConex[i].size() << ". Vertices: ";
        for(int j=0;j<compConex[i].size();j++){
            compoInfoFile << compConex[i][j] << " ";
        }
        compoInfoFile << endl;
    }
    compoInfoFile.close();
}

void gMadj::Dijkstra_m(int u){
    if(fd){
        cout << "Não foi possível rodar o algoritmo de Dijkstra pois o grafo possui aresta com peso negativo." << endl;
    }
    else{
        for(int i=0;i<=m_numVertices;i++) d[i] = INF;
        d[u] = 0;
        using pdi = pair<double,int>;
        priority_queue<pdi,vector<pdi>,greater<pdi>> q;
        q.push({0,u});
        while(!q.empty()){
            int v = q.top().second;
            double d_v = q.top().first;
            q.pop();  
            if(d_v!=d[v])
                continue;

            for(int i=1;i<=m_numVertices;i++){
                if(mAdj[v][i] != 0){
                    int to = i;
                    double len = mAdj[v][i];
                    if(d[v] + len < d[to]){
                        d[to] = d[v] + len;
                        ex[u] = max(ex[u],d[to]);
                        pd[to] = v;
                        q.push({d[to],to});
                    }
                }
            }
        }
    }
}

void gMadj::dist_m(int s,int t){
    ofstream dist;
    dist.open ("DistPath.txt");
    if(td){
        Dijkstra_m(s);
        if(!fd){
            if(d[t] != INF){
                dist << "Distância entre " << s << " e " << t << ": " << d[t] << endl;
                dist << "Caminho: ";
                while(t != s && t != 0 ){
                    dist << t << " ";
                    t = pd[t];
                }
                dist << s << endl;
            }
            else{
                dist << "Distância entre " << s << " e " << t << ": Infinito, pois estão em componentes conexas distintas." << endl;
            }
        }
    }else{
        bfs_m(s,0);
        dist << "Distância entre " << s << " e " << t << ": " << ni[t] << endl;
        dist << "Caminho: ";
        while(t != s && t != 0 ){
            dist << t << " ";
            t = p[t];
        }
        dist << s << endl;    
    }
    dist.close();
}

void gMadj::mst_m(int u){
    double tot = 0;
    for(int i=0;i<=m_numVertices;i++) custo[i] = INF;
    custo[u] = 0;
    using pdi = pair<double,int>;
    vector<bool> processado(m_numVertices+1,false);
    priority_queue<pdi,vector<pdi>,greater<pdi>> q;
    q.push({0,u});
    while(!q.empty()){
        int v = q.top().second;
        double d_v = q.top().first;
        q.pop();
        processado[v] = true;
        for(int i=1;i<=m_numVertices;i++){
            if(mAdj[v][i] != 0){
                int to = i;
                double len = mAdj[v][i];
                if(custo[to]>len && !processado[to]){
                    custo[to] = len;
                    pmst[to] = v;
                    q.push({custo[to],to});
                }
            }
        }
    }
    for(int i=1;i<=m_numVertices;i++){
        if(custo[i]!=INF)
            tot += custo[i];
    }
    ofstream mst;
    mst.open ("MST.txt");
    mst << "Custo da MST: " << tot << endl;
    mst.close();
}

double gMadj::exc_m(int u){
    if(!fd){
        Dijkstra_m(u);
        return ex[u];
    }else{
        cout << "Não foi possível calcular a Excentricidade pois o grafo possui arestas negativas." << endl;
        return 0;
    }
}

bool gMadj::bfs() {
	queue<int> q;

	memset( dist, -1, sizeof(int)*(m_numVertices+1) );
	
    for( int i = 1 ; i <= m_numVertices ; ++i ){
		if( b[i] == -1 ){
			q.push( i ), dist[i] = 0;
        }
    }
    
	bool reached = false;
	while( !q.empty() ) {
		int n = q.front();
		q.pop();
        for(int i=1;i<=m_numVertices;i++){
            if(mAdj[n][i] != 0){
                if( b[i] == -1 ) reached = true;
                else if( dist[b[i]] == -1 ) {
                    dist[b[i]] = dist[n] + 1;
                    q.push( b[i] );
                }
		    }
        }
	}
	return reached;
}

bool gMadj::dfs( int n ) {
	if( n == -1 ) return true;
	for(int i=1;i<=m_numVertices;i++){
        if(mAdj[n][i] != 0){
            if( b[i] == -1 || dist[b[i]] == dist[n] + 1 ) {
                if( dfs( b[i] ) ) {
                    b[i] = n, b[n] = i;
                    return true;
                }
            }
        }
    }
	return false;
}

int gMadj::hk()
{

    bfs_m(1,false);
    //Se a bfs conseguiu bipartir o grafo ele calcula o matching máximo
    if(is_bipartie){
        memset( b, -1,sizeof(int)*(m_numVertices+1));
        
        int ans = 0;
        int k = 0;
        
        while( bfs() ) {
            for( int i = 1 ; i <= m_numVertices ; ++i ){
                if( b[i] == -1 && dfs( i ) ) ++ans;
            }
        }
    
        ofstream infoGrafo;
        infoGrafo.open ("BipartideMatch.txt");
        infoGrafo << "Aresta do Match" << endl;
            for(int i=1;i<=m_numVertices;i++){
                if(vis[i]==0 && b[i]!=-1) infoGrafo << i << " -> " << b[i] << endl;
            }
        infoGrafo.close();
        
        return ans;
    }else{
        printf("Não é bipartido.");
        return 0;
    }
}

void gMadj::BellmanFord(int s){
    bool pcn = false;
    //PCN é uma variável boleana que verifica se possui ciclo negativo e também é usada para ver se o algoritmo pode parar antes da última execução
    
    // Para que o algoritmo de Bellmanford consiga calcular todas as distâncias certinho é necessário que os vértices primeiramente tneham todas as distâncias em infinito
    for(int i=0;i<=m_numVertices;i++){
        dbf[i] = INF;
    }
    // Apenas o vértice de source irá ter sua distância inicial em 0
    dbf[s] = 0;
    for(int i=0;i<m_numVertices-1;i++){
        pcn = true;
        for(int j=1;j<=m_numVertices;j++){
            for(int i=1;i<=m_numVertices;i++){
                if(mAdj[j][i] != 0){
                    int vv = j;
                    double vw = mAdj[j][i];
                    if(dbf[vv]>dbf[j]+vw){
                        pcn = false;
                        dbf[vv] = dbf[j]+vw;
                    }
                }
            }
        }
        if(pcn) break;
    }
    //Aqui verificamos se ao rodarmos mais uma vez o algoritmo de BellmanFord as mudanças mudam. Se mudarem, o grafo possui ciclo negativo.
    pcn = false;
    for(int j=1;j<=m_numVertices;j++){
        for(int i=1;i<=m_numVertices;i++){
            if(mAdj[j][i] != 0){
                int vv = i;
                double vw = mAdj[j][i];
                if(dbf[vv]>dbf[j]+vw){
                    pcn = true;
                    break;
                }
            }
        }
    }
    
    if(pcn) cout << "Possui ciclo negativo." << endl;
    else cout << "Não possui ciclo negativo." << endl;
}

void gMadj::BellmanMatriz(){
    //Para obter a matriz de BellmanFord, rodamos o algoritmo para cada vértice i como source.
    for(int i=1;i<=m_numVertices;i++){
        BellmanFord(i);
    }
    
}
