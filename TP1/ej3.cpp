#include <iostream>
#include <vector>
#include <math.h>
#include <list>

using namespace std;
using Actividad = pair<pair<int, int>, int>;
using Actividades = vector<Actividad>;
using Bucket = vector<list<Actividad>>;

const Actividad UNDEFINED = make_pair(make_pair(0, 0), 0);
const list<Actividad> EMPTY = list<Actividad>();
Actividades actividades_iniciales;
Actividades actividades_maximas;

int n;

void ordenar_actividades() {
	//1 <= si < ti <= 2n
	Bucket bucket = Bucket(2 * n, EMPTY);
	for (long unsigned int i = 0; i < actividades_iniciales.size(); i++) {
		//.first.first para ordenar por si, .first.second para ordenar por ti
		int bucket_correspondiente = actividades_iniciales[i].first.second-1;
		bucket[bucket_correspondiente].push_back(actividades_iniciales[i]);
	}

	int j = 0; //itero actividades_iniciales
	for (int i = 0; i < 2 * n; i++) {
		if (bucket[i].size() > 0) {
			list<Actividad>::iterator it_bucket;
			for (it_bucket = bucket[i].begin(); it_bucket != bucket[i].end(); ++it_bucket) {
				actividades_iniciales[j] = *it_bucket;
				j++;
			}
		}
	}
}

void seleccionar_actividades() {
	actividades_maximas.push_back(actividades_iniciales[0]);
	
	int ti_prev = actividades_iniciales[0].first.second;

	/*
	Como los intervalos están ordenados por si podemos afirmar que si terminan antes que el intervalo seleccionado previamente, su inicio es igual o mayor que el del intervalo anterior.
	Formalmente: sean Si=[si, ti] y Si+1=[si+1, ti+1], HI: si <= si+1, luego si ti+1 <= ti =>  Si+1 se solapa con Si
	*/

	for (long unsigned int i = 1; i < actividades_iniciales.size(); i++) {
		int si_actual = actividades_iniciales[i].first.first;
		bool se_solapan = si_actual < ti_prev;

		if (!se_solapan) {
			//El intervalo actual puede agregarse al conjunto de maximos
			actividades_maximas.push_back(actividades_iniciales[i]);
            ti_prev = actividades_iniciales[i].first.second;
		}		
	}
}

void seleccion_actividades_cardinal_maximo() {
	ordenar_actividades();
	seleccionar_actividades();
}

int main() {
	cin >> n;

	if (n <= 0) return -1;

	for (int i = 0; i < n; i++) {
		int j, k;
		cin >> j >> k;
		actividades_iniciales.push_back(make_pair(make_pair(j, k), i));
	}

	seleccion_actividades_cardinal_maximo();

	int size = actividades_maximas.size();
	cout << size << endl;
	for (int i = 0; i < size; i++) {
		cout << actividades_maximas[i].second + 1 << " ";
	}

	return 0;
}