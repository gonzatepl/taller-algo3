#include <iostream>
#include <vector>
#include <math.h>

using namespace std;
using Cuadrado = vector<vector<int>>;

int n, k, num_magico, k_parcial = 0;

Cuadrado cuadrado;
vector<bool> candidatos;

void setear_num_magico() {
    num_magico = n * (pow(n, 2) + 1) / 2;
}

int suma_fila(Cuadrado& cuadrado, int i, int J) {
    int suma_fila = 0;
    for (int j = 0; j <= J; j++) {
        suma_fila += cuadrado[i][j];
    }
    return suma_fila;
}

bool verificar_filas(Cuadrado& cuadrado) {
    bool res = true;
    for (unsigned int i = 0; i < cuadrado.size() && res; i++) {
        res = res && num_magico == suma_fila(cuadrado, i, n - 1);
    }
    return res;
}

int suma_columna(Cuadrado& cuadrado, int I, int j) {
    int suma_columna = 0;
    for (int i = 0; i <= I; i++) {
        suma_columna += cuadrado[i][j];
    }
    return suma_columna;
}

bool verificar_columnas(Cuadrado& cuadrado) {
    bool res = true;
    for (unsigned int j = 0; j < cuadrado[0].size() && res; j++) {
        res = res && num_magico == suma_columna(cuadrado, n - 1, j);
    }
    return res;
}

void suma_diagonales(Cuadrado& cuadrado, int& sum_diag_ppal, int& sum_diag_sec) {
    for (unsigned int i = 0; i < cuadrado.size(); i++) {
        sum_diag_ppal += cuadrado[i][i];
        sum_diag_sec += cuadrado[i][n - 1 - i];
    }
}

bool verificar_diagonales(Cuadrado& cuadrado) {
    int suma_diagonal_principal = 0;
    int suma_diagonal_secundaria = 0;

    suma_diagonales(cuadrado, suma_diagonal_principal, suma_diagonal_secundaria);

    return suma_diagonal_principal == num_magico &&
           suma_diagonal_secundaria == num_magico;
}

bool es_cuadrado_magico(Cuadrado& cuadrado) {

    return verificar_filas(cuadrado) && verificar_columnas(cuadrado) && verificar_diagonales(cuadrado);
}

void mostrar_cuadrado(Cuadrado& cuadrado) {
    for (unsigned int i = 0; i < cuadrado.size(); i++) {
        for (unsigned int j = 0; j < cuadrado[0].size(); j++) {
            cout << cuadrado[i][j] << " ";
        }
        cout << endl;
    }
}

bool candidatoPasaPodas(int i, int j, int k, int suma_fil, int suma_col){
    bool ultimoFila, ultimoCol, estaUltimoFila, estaUltimoCol;
    if(i == n-1){
       ultimoFila = k == num_magico - suma_col - 1;
       if(!ultimoFila) return false;
    }
    if(j == n-1){
        ultimoCol = k == num_magico - suma_fil - 1;
        if(!ultimoCol) return false;
    }
    if(j == n-2){
        int valor_restante = num_magico - suma_fil - (k+1);
        estaUltimoFila = valor_restante <= pow(n,2) && valor_restante > 0 && candidatos[valor_restante -1];
        if(!estaUltimoFila) return false;
    }
    if(i == n-2){
        int valor_restante = num_magico - suma_col - (k+1);
        estaUltimoCol = valor_restante <= pow(n,2) && valor_restante > 0 && candidatos[valor_restante - 1];
        if(!estaUltimoCol) return false;
    }
    int filaConElemento = suma_fil + k + 1;
    int colConElemento = suma_col + k + 1;
    bool superaFila = (j < n-1 && num_magico < filaConElemento) || (j == n-1 && num_magico != filaConElemento);
    bool superaColumna = (i < n-1 && num_magico <= colConElemento) || (i == n-1 && num_magico != colConElemento);

    return !superaFila && !superaColumna;

}

// si retorna 0 es que el algoritmo encontró el cuadrado mágico k-esimo de orden n
// si retorna 1 es que el algoritmo encontró un nuevo cuadrado mágico k'-esimo de orden n (0 < k' < k)
// si retorna -1 es que el cuadrado parcial que estaba buscando dejó de ser un candidato posible para ser cuadrado mágico de orden n
int cuadrado_k_orden_n(int i, int j) {
    if (i == n) {
        if (es_cuadrado_magico(cuadrado)) {
            k_parcial++;
            if (k_parcial == k) {
                //Hemos hallado el k-esimo cuadrado mágico de orden n ordenado lexicográficamente
                //Lo mostramos
                mostrar_cuadrado(cuadrado);
                return 0;
            }
            return 1;
        }
        else {
            return -1;
        }
    }

    int suma_col=0, suma_fil=0;

    if (!(i == 0 && j == 0)) {
        suma_col = suma_columna(cuadrado, i, j);
        suma_fil = suma_fila(cuadrado, i, j);
    }

    int prox_j = j == n - 1 ? 0 : j + 1;
    int prox_i = prox_j == 0 ? i + 1 : i;


    for (unsigned int k = 0; k < candidatos.size(); k++) {
        //que sea candidato no es solo que no esté en el cuadrado aún -> es que sea factible incluirlo
        if(candidatos[k] && candidatoPasaPodas(i,j,k,suma_fil,suma_col)){
            cuadrado[i][j] = k+1;

            //borro el elemento de la posición k-esima del actual vector de candidatos
            candidatos[k] = 0;

            if (cuadrado_k_orden_n(prox_i, prox_j) == 0) {
                //Encontré el cuadrado mágico k-esimo de orden n
                candidatos[k] = 1; //VER SI SE PUEDE NO REPETIR ESTA LINEA
                return 0;
            }

            candidatos[k] = 1;	//VER SI SE PUEDE NO REPETIR ESTA LINEA
        }
    }
    cuadrado[i][j] = 0;
    return 2;
}

int encontrar_cuadrado_k_orden_n() {
    return cuadrado_k_orden_n(0, 0);
}

void inicializar_cuadrado() {
    cuadrado = Cuadrado(n, vector<int>(n));
}

void inicializar_candidatos() {
    candidatos = vector<bool>(pow(n, 2),1);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n >> k;
    inicializar_cuadrado();
    setear_num_magico();
    inicializar_candidatos();

    if (encontrar_cuadrado_k_orden_n() != 0) {
        cout << "-1" << endl;
    }

    return 0;
}
