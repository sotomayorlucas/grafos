#include <cstdio>
#include <vector>
#include <queue>
#include <climits>
#include <bitset>

using namespace std;

struct Arista {
    int v;
    int capacidad;
    int indiceReverso;  // Índice del reverso en la lista de adyacencia del nodo destino
};

class Grafo {
private:
    vector<vector<Arista>> grafo;
    vector<int> padre;

public:
    Grafo(int n) {
        grafo.resize(n + 2);
        padre.resize(n + 2, -1);
    }

    void agregarArista(int u, int v, int capacidad) {
        grafo[u].push_back({v, capacidad, static_cast<int>(grafo[v].size())});
        grafo[v].push_back({u, capacidad, static_cast<int>(grafo[u].size()) - 1});
    }

    bool bfs(int fuente, int sumidero) {
        int n = grafo.size();
        bitset<301> visitado;  // BitSet para marcar los nodos visitados
        queue<int> cola;
        cola.push(fuente);
        visitado.set(fuente);

        while (!cola.empty()) {
            int u = cola.front();
            cola.pop();

            for (const auto& arista : grafo[u]) {
                int v = arista.v;
                int capacidad = arista.capacidad;

                if (!visitado.test(v) && capacidad > 0) {
                    padre[v] = u;
                    cola.push(v);
                    visitado.set(v);

                    if (v == sumidero) {
                        return true;
                    }
                }
            }
        }

        return false;
    }

    int flujoMaximo(int fuente, int sumidero) {
        int flujoMax = 0;

        while (bfs(fuente, sumidero)) {
            int flujoCamino = INT_MAX;
            int v = sumidero;

            while (v != fuente) {
                int u = padre[v];

                for (auto& arista : grafo[u]) {
                    if (arista.v == v) {
                        flujoCamino = min(flujoCamino, arista.capacidad);
                        break;
                    }
                }

                v = u;
            }

            v = sumidero;
            while (v != fuente) {
                int u = padre[v];

                for (auto& arista : grafo[u]) {
                    if (arista.v == v) {
                        arista.capacidad -= flujoCamino;
                        grafo[v][arista.indiceReverso].capacidad += flujoCamino;
                        break;
                    }
                }

                v = u;
            }

            flujoMax += flujoCamino;
        }

        return flujoMax;
    }
};

int calcularMinimoDesacuerdos(int n, vector<int>& creencias, vector<pair<int, int>>& amigos) {
    int fuente = 0;            // Nodo fuente
    int sumidero = n + 1;      // Nodo sumidero
    Grafo grafo(sumidero + 1);

    for (auto& amigo : amigos) {
        int u = amigo.first;
        int v = amigo.second;

        grafo.agregarArista(u, v, 1);
    }

    for (int i = 1; i <= n; i++) {
        if (creencias[i - 1] == 1) {
            // El chico i cree en que Kruskal es mejor
            grafo.agregarArista(fuente, i, 1);
        } else {
            // El chico i cree que Prim es mejor
            grafo.agregarArista(i, sumidero, 1);
        }
    }

    int flujoMaximo = grafo.flujoMaximo(fuente, sumidero);

    return flujoMaximo;
}

int main() {
    while (true) {
        int n, m;
        scanf("%d %d", &n, &m);

        if (n == 0 && m == 0) {
            break;
        }

        vector<int> creencias(n);
        vector<pair<int, int>> amigos(m);

        // Leer las creencias de los amigos
        for (int i = 0; i < n; i++) {
            scanf("%d", &creencias[i]);
        }

        // Leer las amistades
        for (int i = 0; i < m; i++) {
            scanf("%d %d", &amigos[i].first, &amigos[i].second);
        }

        // Calcular el mínimo número de desacuerdos
        int minimoDesacuerdos = calcularMinimoDesacuerdos(n, creencias, amigos);
        printf("%d\n", minimoDesacuerdos);
    }

    return 0;
}
