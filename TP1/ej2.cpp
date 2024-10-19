
#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

long long n, r, m;
vector<long long> nros;

const long long UNDEFINED = -1;

vector<vector<long long>> memo;

long long modulo(long long n){
    return ((n % m) + m) % m;
}

long long modex(long long w, long long e){
    w %= m;
    if (w < 0) w += m;
    long long res = 1;
    while(e > 0) {
        if ((e & 1) > 0) res = (res * w) % m;
        e/=2;
        w = (w*w) % m;
    }
    return res;
}

bool es_posible_aux(long long w, long long i){
    if(i==n) return w % m == r;
    if(memo[i][w] != UNDEFINED) return memo[i][w];
    memo[i][w] =  es_posible_aux((w + nros[i]) % m, i+1) or
                  es_posible_aux(modulo(w - nros[i]), i+1) or
                  es_posible_aux((w * nros[i]) % m, i+1) or
                  es_posible_aux(modex(w,nros[i]), i+1);
    return memo[i][w];
}

bool es_posible(){
    return es_posible_aux(nros[0], 1);
}

void inicializar_memoizacion(){
    memo = vector<vector<long long>>(n, vector<long long>(m,UNDEFINED));
}

int main() {
//Truco para optimizar cin y cout
    ios::sync_with_stdio();
    cin.tie(NULL);
   
    int c, i=0;

    cin >> c;
    vector<bool> res(c);
    while (i < c){
        cin >> n >> r >> m;
        inicializar_memoizacion();
        nros =  vector<long long>(n);
        for(int j=0; j<n; j++){
            cin>>nros[j];      
        }
        res[i] = es_posible();
        i++;        
    }
    for(int j = 0; j < res.size(); j++){
        if(res[j]) cout << "Si \n";
        else cout << "No \n";
    }
    return 0;
}
