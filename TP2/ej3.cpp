#include <iostream>
#include <vector>
#include <math.h>
#include <unordered_map>
#include <iomanip>
#include <algorithm>
#include <tuple> 

using namespace std;

int C, N, R, W, U, V;
//Grafo grafo;
vector<pair<int, int>> posiciones_oficinas;
vector<pair<double, double>> resultados;

vector<tuple<double, int, int>> cables_totales;
vector<tuple<double, int, int>> cables_necesarios;

struct DSU {

    //DSU - Naive (sin path compression)

    DSU(int n) {
        padre = vector<int>(n);
        for (int v = 0; v < n; v++) padre[v] = v;
    }

    //find: Devuelve el parent
    int find(int v) {
        if (v == padre[v]) return v;
        return padre[v] = find(padre[v]);  
    } //Ahora la busqueda del padre de v en peor caso es lineal

    //unite: Hace la unión y actualiza el tamaño del resultante
    void unite(int u, int v) {
        u = find(u), v = find(v);
        padre[u] = v;
    }

    vector<int> padre; //p: Vector con los parents
};

void kruskal() {
    sort(cables_totales.begin(), cables_totales.end());

    int cant_aristas = 0;
    DSU dsu(N); //Indexo desde 0
    for (auto [d, u, v] : cables_totales) {
        if (dsu.find(u) != dsu.find(v)) {

            cant_aristas++;
            if (cant_aristas == N - 1) break;

            dsu.unite(u, v);

            // agrego el cable que conecta las oficinas u y v al conjunto de cables elegidos para la conexion    
            cables_necesarios.push_back({ d, u, v });
        }
    }
}

void crear_grafo_oficinas() {

    ////Inicializo las N+1 vecindades (una por cada oficina + la que agregaremos para el modelo del AGM)
    //for (int i = 0; i <= N; i++) {
    //    grafo[i] = vector<pair<int, double>>();
    //}

    //Creo las n(n-1)/2 aristas iniciales con el peso de colocar un cable entre cada par de oficinas (la distancia entre ambas oficinas, medida en cms)
    int x1, x2, y1, y2, c1, c2;
    double d;
    for (int i = 0; i < N; i++) {
        x1 = posiciones_oficinas[i].first;
        y1 = posiciones_oficinas[i].second;
        for (int j = i + 1; j < N; j++) {
            x2 = posiciones_oficinas[j].first;
            y2 = posiciones_oficinas[j].second;

            c1 = abs(x1 - x2);
            c2 = abs(y1 - y2);

            d = sqrt(pow(c1, 2) + pow(c2, 2));

            cables_totales.push_back({ d, i, j });
        }
    }
}

void calcular_costos() {
    double total_cables_UTP = 0;
    double total_cables_fibra = 0;

    //Si tengo N oficinas y W modems, con N-1-(W-1) = N-W conexiones puedo asegurar que a ninguna oficina le faltará internet
    //(Con N-1 conexiones y un modem, las N oficinas tienen internet, con N-2 conexiones y dos modems las N oficinas tienen
    //internet, y asi...)

    //En cables_necesarios ya tengo las N-1 conexiones entre oficinas de menor costo, ordenadas de forma ascendente por este.
    //Luego, si quiero el costo minimo para las N-W conexiones que conecten las oficinas, tomo las primeras N-W conexiones de
    //las ya calculadas
    for (int i = 0; i < N - W; i++) {
        double costo_cable = get<0>(cables_necesarios[i]);
        if (costo_cable <= R) total_cables_UTP += U * costo_cable;
        else  total_cables_fibra += V * costo_cable;
    }

    resultados.push_back(make_pair(total_cables_UTP, total_cables_fibra));
}

int main() {
    //Truco para optimizar cin y cout
    ios::sync_with_stdio();
    cin.tie(NULL);

    //Leemos del input y generamos el grafo como lista de adyacencias pesadas y lista de aristas pesadas
    cin >> C;

    for (int i = 0; i < C; i++) {
        //Por cada caso de test seteamos nuevamente estas variables
        cin >> N >> R >> W >> U >> V;
        //Inicializamos nuevamente el grafo (VER SI ES CORRECTO)
        //grafo = {};
        cables_totales = vector<tuple<double, int, int>>();
        cables_necesarios = vector<tuple<double, int, int>>();
        posiciones_oficinas = vector<pair<int, int>>(N);

        for (int j = 0; j < N; j++) {
            //Obtenemos las N posiciones de las oficinas
            int x, y;
            cin >> x >> y;
            posiciones_oficinas[j] = make_pair(x, y);
        }

        //A partir de las posiciones de las N oficinas creamos las aristas entre todas las oficinas
        crear_grafo_oficinas();

        //Corro el algoritmo de Kruskal para obtener el AGM del grafo que modela la oficinas y el costo de las conexiones entre ellas
        kruskal();

        //A partir de Kruskal obtuve las  N-1 conexiones más económicas que necesito entre las oficinas para que a ninguna
        //le falte internet si contamos con un único modem (que puede estar en cualquiera de estas N oficinas)
        //Ahora debemos obtener los costos de las mínimas conexiones que se necesitan si contamos con W modems para las N oficinas
        calcular_costos();

    }

    //Imprimo por pantalla los C resultados
    for (int i = 0; i < C; i++) {
        double costo_utp = resultados[i].first, costo_fibra = resultados[i].second;
        //double costo_utp = 1.23456563, costo_fibra = 0.2345234234;

        cout << "Caso #" << i + 1 << ": ";
        cout << fixed;
        cout << setprecision(3) << costo_utp << " ";
        cout << setprecision(3) << costo_fibra << "\n";
    }

    return 0;
}