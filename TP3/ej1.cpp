#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <tuple> 
#include <queue>

using namespace std;

using Grafo = vector<vector<pair<int, int>>>;
using AristaPesada = tuple<int, int, int>;
using AristasPesadas = vector<AristaPesada>;
using Distancias = vector<int>;
int INF;

int C, N, M, K, S, T;

void Dijkstra(const Grafo& ady, Distancias& distancias, int v) {
    //Implementacion que dieron en la clase practica

    priority_queue<pair<int, int>> q;
    //queue<pair<int, int>> q; //Cola común
    vector<bool> procesados = vector<bool>(N + 1, false);

    for (int i = 1; i <= N; i++) distancias[i] = INF; //Inicializo todas las distancias en INF

    distancias[v] = 0; q.push({ 0,v }); //La distancia de v consigo mismo es 0. Lo pusheo a la cola

    while (!q.empty()) {
        int a = q.top().second /* q.front().second //si se utiliza cola comun */; q.pop();
        if (procesados[a]) continue;
        procesados[a] = true;
        for (auto u : ady[a]) {
            int b = u.first, w = u.second;
            if (distancias[a] + w < distancias[b]) {
                distancias[b] = distancias[a] + w;
                q.push({ -distancias[b],b });
            }
        }
    }

}

void crear_grafo_ciudad(Grafo& ciudad, Grafo& ciudad_invertida, AristasPesadas& calles_candidatas) {

    //Los creo de N+1 posiciones asi se mantienen las relaciones entre los nodos 1 a N
    ciudad = Grafo(N + 1);
    ciudad_invertida = Grafo(N + 1); // grafo con las direcciones de las calles invertidas (traspuesto)

    INF = 0;

    for (int i = 0; i < M; i++) {
        int d, c, l;
        cin >> d >> c >> l;

        //Grafos de vecindades de salida
        ciudad[d].push_back({ c, l });
        ciudad_invertida[c].push_back({ d, l });

        INF += l;
    }

    INF++; //Infinito termina con la sumatoria de las longitudes de las calles de la ciudad, incrementado en una unidad. De esta forma
           //nos garantizamos de que no haya overflow al calcular las distancias y al evaluar candidatas, que pueda alterar los 
           //resultados (ningun camino va a tener más que la sumatoria de todas las longitudes de las calles incrementadas en una unidad)
    

    //Agrego las candidatas bidireccionales ci a mejorar la distancia entre s y t
    //calles_candidatas las toma de forma unidireccional: Cada ci = (u,v) se agrega como u->v y v->u, cada una con longitud (ci)
    for (int i = 0; i < K; i++) {
        int u, v, q;
        cin >> u >> v >> q;

        //agrego u->v y v->u, cada una con longitud q
        calles_candidatas.push_back({ u, v, q });
        calles_candidatas.push_back({ v, u, q });
    }
}

int longitud_minima_posible(const Distancias& dS, const Distancias& dT, const AristasPesadas& calles_candidatas) {
    int res = dS[T];

    for (AristaPesada a : calles_candidatas) {
        int v = get<0>(a), w = get<1>(a), l = get<2>(a);
        int nuevaLongitud = dS[v] + l + dT[w];
        if (nuevaLongitud < res)
            res = nuevaLongitud;
    }

    return res == INF ? -1 : res;
}

int main() {
    //Truco para optimizar cin y cout
    ios::sync_with_stdio();
    cin.tie(NULL);

    vector<int> resultados = vector<int>();

    //Leemos del input y generamos el grafo como lista de adyacencias pesadas y lista de aristas pesadas
    cin >> C;

    if (C > 20) return -1;

    for (int i = 0; i < C; i++) {

        cin >> N >> M >> K >> S >> T;

        if (S > N || T > N) return -1;

        Grafo ciudad, ciudad_invertida;
        AristasPesadas calles_candidatas;

        crear_grafo_ciudad(ciudad, ciudad_invertida, calles_candidatas);


        Distancias dS = vector<int>(N + 1);
        Distancias dT = vector<int>(N + 1);

        //Corro el algoritmo de Dijkstra para obtener las distancias entre S y todos los demas puntos de la ciudad
        Dijkstra(ciudad, dS, S);
        //Corro el algoritmo de Dijkstra para obtener las distancias entre todos los demas puntos de la ciudad y T
        Dijkstra(ciudad_invertida, dT, T);

        resultados.push_back(longitud_minima_posible(dS, dT, calles_candidatas));
    }

    //Imprimo por pantalla los C resultados
    for (int i = 0; i < C; i++) cout << resultados[i] << "\n";

    return 0;
}