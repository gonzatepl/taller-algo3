#include <iostream>
#include <vector>
#include <math.h>
#include <stack>
#include <algorithm>
#include <string>
 
using namespace std;
using Grafo = vector<vector<int>>;
 
int n,m;
int cfc = -1;
 
void DFS(Grafo &grafo, int v, vector<int> &visitado, stack<int> &finalizacion){
    for(int u: grafo[v]){
        if(!visitado[u]){
            visitado[u] = true;
            DFS(grafo,u,visitado,finalizacion);
        }
    }
    finalizacion.push(v); 
}

void CFC(Grafo& transpuesto, int v, vector<int> &visitado, vector<int>& cfcs){
    for(int u: transpuesto[v]){
        if(!visitado[u]){
            visitado[u] = true;
            cfcs[u] = cfc;
            CFC(transpuesto, u, visitado, cfcs);
        }
    }
}
 
int main() {
    //Truco para optimizar cin y cout
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    //Leemos del input y generamos el digrafo y el transpuesto
    cin >> n >> m;
    Grafo grafo(n);
    Grafo transpuesto(n);
    for(int i = 0; i < m; i++){
        int u,v;
        cin >> u >> v;
        grafo[u-1].push_back(v-1);
        transpuesto[v-1].push_back(u-1);
    }
    //Generamos en un primer DFS los tiempos de finalizacion de cada nodo
    vector<int> visitado(n);
    stack<int> finalizacion;
    for(int i = 0; i < n; i++){
        if(!visitado[i]){
            visitado[i] = true;
            DFS(grafo, i, visitado, finalizacion);
        }
    }
    visitado = vector<int>(n,false);
    //Creamos vector auxiliar para almacenar la cfc de cada vertice
    vector<int> cfcs(n);
    //Generamos en un segundo DFS las CFCs usando los ordenes descendentes de finalización
    for(int i = 0; i < n; i++){
        int v = finalizacion.top();
        finalizacion.pop();
        if(!visitado[v]){
            visitado[v] = true;
            cfc++;
            cfcs[v] = cfc;
            CFC(transpuesto, v, visitado, cfcs);
        }
    }
    //Recorremos el grafo para descartar CFCs cuyo grado de entrada es mayor a 0 y por tanto
    //ya no podrían ir al óptimo
    vector<int> entrada_cero(n, true);
    for(int v = 0; v < n; v++){
        for(int w : grafo[v]){
            if(cfcs[v] != cfcs[w]) entrada_cero[w] = false;
        }
    }
    //Almacenamos en tiempo lineal el minimo v de cada CFC y anulamos los no minimos de las CFCs,
    //o los minimos que estan en CFCs que ya fueron anuladas previamente
    vector<int> min_cfc(cfc+1,-1);
    for(int v = 0; v < n; v++){
        if(min_cfc[cfcs[v]] == -1) min_cfc[cfcs[v]] = v;
        else {
            if(entrada_cero[v]) entrada_cero[v] = false;
            else entrada_cero[min_cfc[cfcs[v]]] = false;
        }
    }
    //Consideramos para el óptimo aquellos v en CFCs quedaron con entrada_cero = true que son a su 
    //vez los minimos de la CFC
    int tam_optimo = 0;
    for(int i = 0; i < n; i++) if(entrada_cero[i]) tam_optimo++;
    cout << tam_optimo << endl;
    for(int i = 0; i < n; i++){
        if(entrada_cero[i]) cout << i+1 << " ";
    }
    cout << endl;
} 