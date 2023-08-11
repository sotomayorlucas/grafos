#include <cstdio>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

typedef pair<int, int> pii;

const int INF = INT_MAX;

// Función que implementa el algoritmo de Dijkstra para encontrar el camino más corto en el grafo
int dijkstra(vector<vector<pii>>& grafo, int inicio, int fin) {
    int n = grafo.size();
    vector<int> distancias(n, INF);
    distancias[inicio] = 0;

    priority_queue<pii, vector<pii>, greater<pii>> pq;
    pq.push(make_pair(0, inicio));

    while (!pq.empty()) {
        int dist = pq.top().first;
        int nodo = pq.top().second;
        pq.pop();

        // Si se alcanza el nodo de destino, se retorna la distancia mínima
        if (nodo == fin)
            return distancias[fin];

        // Si la distancia actual es mayor que la distancia almacenada, se ignora
        if (dist > distancias[nodo])
            continue;

        // Explorar los vecinos del nodo actual
        for (const auto& vecino : grafo[nodo]) {
            int siguienteNodo = vecino.first;
            int peso = vecino.second;
            int siguienteDist = dist + peso;

            // Si la distancia actualizada es menor a la distancia almacenada, se actualiza la distancia y se añade a la cola de prioridad
            if (siguienteDist < distancias[siguienteNodo]) {
                distancias[siguienteNodo] = siguienteDist;
                pq.push(make_pair(siguienteDist, siguienteNodo));
            }
        }
    }

    // Si no se encuentra un camino desde el nodo de inicio al nodo de destino, se retorna -1
    return -1;
}

// Función que resuelve el problema de la red de tráfico
int resolver_red_trafico(vector<vector<int>>& datos) {
    int n = datos[0][0];
    int m = datos[0][1];
    int k = datos[0][2];
    int s = datos[0][3];
    int t = datos[0][4];

    vector<vector<pii>> grafo(n + 1);

    // Construir el grafo a partir de los datos de entrada
    for (int i = 1; i <= m; i++) {
        int d = datos[i][0];
        int c = datos[i][1];
        int l = datos[i][2];
        grafo[d].push_back(make_pair(c, l));
    }

    int longitudMinima = INF;

    // Iterar sobre las propuestas de carreteras bidireccionales
    for (int i = m + 1; i <= m + k; i++) {
        int u = datos[i][0];
        int v = datos[i][1];
        int q = datos[i][2];

        // Añadir la carretera propuesta al grafo
        grafo[u].push_back(make_pair(v, q));
        grafo[v].push_back(make_pair(u, q));

        // Calcular la longitud del camino más corto después de construir la carretera
        int longitudCamino = dijkstra(grafo, s, t);
        longitudMinima = min(longitudMinima, longitudCamino);

        // Eliminar la carretera del grafo
        grafo[u].pop_back();
        grafo[v].pop_back();
    }

    // Retornar la longitud mínima del camino más corto
    return longitudMinima;
}

int main() {
    int numConjuntosDatos;
    scanf("%d", &numConjuntosDatos);

    for (int i = 0; i < numConjuntosDatos; i++) {
        int n, m, k, s, t;
        scanf("%d %d %d %d %d", &n, &m, &k, &s, &t);

        vector<vector<int>> datos(m + k + 1, vector<int>(3));

        datos[0] = { n, m, k, s, t };

        // Leer los datos de las carreteras unidireccionales
        for (int j = 1; j <= m; j++) {
            scanf("%d %d %d", &datos[j][0], &datos[j][1], &datos[j][2]);
        }

        // Leer los datos de las carreteras bidireccionales propuestas
        for (int j = m + 1; j <= m + k; j++) {
            scanf("%d %d %d", &datos[j][0], &datos[j][1], &datos[j][2]);
        }

        // Resolver el problema de la red de tráfico y obtener el resultado
        int resultado = resolver_red_trafico(datos);

        // Imprimir el resultado
        printf("%d\n", resultado);
    }

    return 0;
}
