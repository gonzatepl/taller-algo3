#include <iostream>
#include <vector>
#include <math.h>
#include <iomanip>
#include <algorithm>
 
using namespace std;
using Grafo = vector<vector<int>>;
 
int n,m;
int cc = 0;
vector<pair<int,int>> aristas;
vector<pair<int,int>> puentes;
vector<int> min_nivel_alcanzable;
vector<int> comp_conexa;
vector<int> tam_componente;
 
long long combinatorio(int n, int k){
    if(k > n) return 0;
    if(k == 0 or k==n) return 1;
    long long comb = 1;
    for(int i = 1; i <= k; i++){
        comb = comb * (n-i+1) / i;
    }
    return comb;
}
 
void DFS(Grafo &grafo, int v, int padre, vector<bool> &visitado, vector<int> &level){
    if(padre != -1) level[v] = level[padre]+1, min_nivel_alcanzable[v] = level[v];
    else level[v] = min_nivel_alcanzable[v] = 0;
    for(int u: grafo[v]){
        if(!visitado[u]){
            visitado[u] = true;
            comp_conexa[u] = cc;
            DFS(grafo,u,v, visitado, level);
            min_nivel_alcanzable[v] = min(min_nivel_alcanzable[v],min_nivel_alcanzable[u]);
            if(level[v] < min_nivel_alcanzable[u]){
                if(u >= v) puentes.push_back(make_pair(v,u));
                else puentes.push_back(make_pair(u,v));
            }
        } else if(u != padre){
            min_nivel_alcanzable[v] = min(min_nivel_alcanzable[v], level[u]);
        }
    } 
}
 
void obtener_puentes(Grafo &grafo){
      vector<bool> visitado(n);
      vector<int> level(n);
      visitado[0] = true;
      DFS(grafo, 0, -1, visitado, level);
}
 
Grafo quitarPuentes(){
    Grafo sinPuentes(n);
    for(unsigned int i = 0; i < aristas.size(); i++){
        int u = aristas[i].first;
        int v = aristas[i].second;
        if(find(puentes.begin(), puentes.end(), aristas[i]) == puentes.end()){
            sinPuentes[u].push_back(v);
            sinPuentes[v].push_back(u);
        }
    }
    return sinPuentes;
}
 
void calcular_componentes(Grafo &grafo){
    vector<bool> visitado(n);
    vector<int> level(n);
    for (int i = 0; i < n; i++) {
        if(!visitado[i]){
            cc++;
            visitado[i] = true;
            comp_conexa[i] = cc;
            DFS(grafo, i, -1, visitado, level);
        }
    }
	tam_componente.resize(cc+1,0);
    for(int i = 0; i < n; i++) {
        tam_componente[comp_conexa[i]]++;
    }
}
 
long long calcularFormasPerder(){ 
    long long formas_perder = 0;
    for(int i = 0; i <= cc; i++){ 
        long long tam = tam_componente[i];
        formas_perder+= tam * (n - tam);
    }
    //como contamos dos veces cada par, se que es par seguro formas_perder, asi que lo dividimos por 2
    return formas_perder/2;
}
 
 
int main() {
//Truco para optimizar cin y cout
    ios::sync_with_stdio();
    cin.tie(NULL);
    
    //Leemos del input y generamos el grafo como lista de adyacencias y lista de aristas
    cin >> n >> m;
    Grafo grafo(n);
    for(int i = 0; i < m; i++){
        int u,v;
        cin >> u >> v;
        if(v>=u) aristas.push_back(make_pair(u-1,v-1));
        else aristas.push_back(make_pair(v-1,u-1));
        grafo[u-1].push_back(v-1);
        grafo[v-1].push_back(u-1);
    }
	//Resizeamos vectores
	min_nivel_alcanzable.resize(n), comp_conexa.resize(n);
    //Calculamos las formas de jugar como el combinatorio entre (n 2)
    long long formas_jugar = combinatorio(n,2);
    //Obtenemos los puentes del grafo con DFS
    obtener_puentes(grafo);
    //Rearmamos G quitandole los puentes para calcular en que componente conexa se encuentra cada nodo ahora
    Grafo sinPuentes = quitarPuentes();
    calcular_componentes(sinPuentes);
    //Calculamos las formas de perder
    long long formas_perder = calcularFormasPerder();
    //Calculamos y mostramos la probabilidad con 5 decimales
    cout << fixed << setprecision(5) << (1.0 * formas_perder / formas_jugar);
    return 0;
}  