#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std;

class DSU{
private:
    vector<int> padre;
    vector<int> rango;
public:
    DSU(int tam) {
        padre.resize(tam);
        rango.resize(tam, 0);
        for (int i = 0; i < tam; i++) {
            padre[i] = i;
        }
    }

    int encontrarRaiz(int nodo) {
        if (nodo != padre[nodo]) {
            padre[nodo] = encontrarRaiz(padre[nodo]);
        }
        return padre[nodo];
    }

    void unirConjuntos(int nodoA, int nodoB) {
        int raizA = encontrarRaiz(nodoA);
        int raizB = encontrarRaiz(nodoB);
        if (rango[raizA] < rango[raizB]) {
            padre[raizA] = raizB;
        } else if (rango[raizA] > rango[raizB]) {
            padre[raizB] = raizA;
        } else {
            padre[raizA] = raizB;
            rango[raizB]++;
        }
    }
};



class Oficina {
public:
    int x;
    int y;
    Oficina(int x, int y) : x(x), y(y) {}
};

class Cable {
public:
    int i;
    int j;
    double distancia;
    Cable(int i, int j, double d) : i(i), j(j), distancia(d) {}
    bool operator<(const Cable& other) const {
        return distancia < other.distancia;
    }

};

class Pabellon {
public:
    int cantidadOficinas;
    int limiteUTP;
    int cantidadModems;
    int precioUTP;
    int precioFibra;
    vector<Oficina> oficinas;
    vector<Cable> vCables;

    Pabellon(int N, int R, int W, int U, int V) : cantidadOficinas(N), limiteUTP(R), cantidadModems(W), precioUTP(U), precioFibra(V) {}

    void agregarOficina(int x, int y) {
        oficinas.emplace_back(x, y);
    }

    void agregarCable(int i, int j, double distancia) {
        vCables.emplace_back(i, j, distancia);
    }

    double calcularDistancia(int x1, int y1, int x2, int y2) {
        int dx = x2 - x1;
        int dy = y2 - y1;
        return sqrt(dx * dx + dy * dy);
    }

    pair<double, double> calcularCostoTotal() {
        sort(vCables.begin(), vCables.end());
        DSU dsu(cantidadOficinas);
        double distanciaUTP = 0.0;
        double distanciaFibraOptica = 0.0;
        int componentes = cantidadOficinas;
        for (const Cable &cable: vCables) {
            int iRoot = dsu.encontrarRaiz(cable.i);
            int jRoot = dsu.encontrarRaiz(cable.j);
            if (iRoot != jRoot) {
                dsu.unirConjuntos(cable.i,cable.j);
                if (precioUTP <= precioFibra && cable.distancia <= limiteUTP) {
                    distanciaUTP += cable.distancia;
                } else {
                    distanciaFibraOptica += cable.distancia;
                }
                componentes--;
                if (componentes == cantidadModems) {
                    break;
                }
            }
        }

        return make_pair(distanciaUTP * precioUTP, distanciaFibraOptica * precioFibra);
    }
};

int main() {
    int c;
    cin >> c;

    for (int casoPrueba = 1; casoPrueba <= c; casoPrueba++) {
        int N, R, W, U, V;
        cin >> N >> R >> W >> U >> V;

        Pabellon pabellon(N, R, W, U, V);

        for (int i = 0; i < N; i++) {
            int x, y;
            cin >> x >> y;
            pabellon.agregarOficina(x, y);
        }

        for (int i = 0; i < N; i++) {
            for (int j = i + 1; j < N; j++) {
                double distancia = pabellon.calcularDistancia(pabellon.oficinas[i].x, pabellon.oficinas[i].y,
                                                              pabellon.oficinas[j].x, pabellon.oficinas[j].y);

                pabellon.agregarCable(i, j, distancia);
            }
        }
        pair<double, double> costos = pabellon.calcularCostoTotal();
        cout << "Caso #" << casoPrueba << ": "<< fixed << setprecision(3) << costos.first << " " << costos.second <<endl;
    }

    return 0;
}
