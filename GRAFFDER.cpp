    #include <iostream>
#include <algorithm>
    #include <vector>
    #include <iomanip>

    using namespace std;

    void eliminarElemento(vector<int>& v, const int& elemento) {
        v.erase(remove(v.begin(), v.end(), elemento), v.end());
    }

    long double combinatorio(int n, int k) {
        std::vector<std::vector<long double>> dp(n + 1, std::vector<long double>(k + 1, 0));
        // Casos base: C(n, 0) = C(n, n) = 1
        for (auto i = 0; i <= n; i++) {
            dp[i][0] = 1.0;
            if (i<=k)    dp[i][i] = 1.0;
        }
        // Calcular los valores utilizando la relación C(n, k) = C(n-1, k-1) + C(n-1, k)
        for (auto i = 1; i <= n; i++) {
            for (auto j = 1; j <= k; j++) {
                dp[i][j] = dp[i - 1][j - 1] + dp[i - 1][j];
            }
        }
        return dp[n][k];
    }

    class Grafo {
        int N; // Número de nodos
        vector<vector<int>> adyacentes; // Lista de adyacencia
        vector<int> estado; // Registro de nodos visitados
        vector<vector<int>> treeEdges; // Almacena las adyacentes de árbol
        int NO_LO_VI = 0, EMPECE_A_VER = 1, TERMINE_DE_VER = 2;
        vector<int> backConExtremoInferiorEn;
        vector<int> backConExtremoSuperiorEn;
        vector<int> memo;
        vector<pair<int,int>> puentes;
        vector<int> contadorComponentes;
        ;

    public:
        Grafo(int N);
        void AgregarArista(int u, int v);
        void DFSconTreeYBackEdges(int nodo, int padre);
        int cubren(int v, int p);
        void verticeConPuente();
        void resetEstado();
        long double posibilidadDePerder();
        void DFS();
        void quitarPuentes();
        void dfsElementosConexos(int nodo, int &contador);
        void contarComponentesConexas();
    };
    Grafo::Grafo(int N) {
        this->N = N;
        adyacentes.resize(N + 1);
        estado.resize(N + 1, NO_LO_VI);
        treeEdges.resize(N+1);
        backConExtremoInferiorEn.resize(N+1,0);
        backConExtremoSuperiorEn.resize(N+1,0);
        memo.resize(N+1,-1);
    }

    void Grafo::AgregarArista(int u, int v) {
        adyacentes[u].push_back(v);
        adyacentes[v].push_back(u);
    }


    void Grafo::DFS(){
        for(int i = 1; i<=N; i++){
            if (estado[i] == NO_LO_VI){
                DFSconTreeYBackEdges(i,-1);
            }
        }
    }
    void Grafo::DFSconTreeYBackEdges(int nodo, int padre) {
        estado[nodo] = EMPECE_A_VER;
        for (int u : adyacentes[nodo]) {
            if (estado[u] == NO_LO_VI) {
                treeEdges[nodo].push_back(u);
                DFSconTreeYBackEdges(u, nodo);
            } else if (u != padre && padre != -1) {
                if (estado[u] != TERMINE_DE_VER){
                    backConExtremoInferiorEn[nodo]++;
                    backConExtremoSuperiorEn[u]++;
                }
            }
        }
        estado[nodo] = TERMINE_DE_VER;
    }

        int Grafo::cubren(int v, int p) {
        if (memo[v] != -1) return memo[v];
        int res = 0;
        res += backConExtremoInferiorEn[v];
        res -= backConExtremoSuperiorEn[v];
        vector<int> posiblesPuentes;
        for (int hijo : treeEdges[v]) {
            if (hijo != p) {
                int hijoRes = cubren(hijo,v);
                if ( hijoRes == 0){
                    puentes.emplace_back(v,hijo);
                }
                res += hijoRes;
            }
        }
        memo[v] = res;
        return res;
    }


    void Grafo::dfsElementosConexos(int nodo, int& contador){
        estado[nodo] = true;
        contador++;
        for (int vecino : adyacentes[nodo]){
            if(estado[vecino] == NO_LO_VI){
                dfsElementosConexos(vecino, contador);
            }
        }
    }

    void Grafo::contarComponentesConexas() {
        for (int i = 0; i < N; i++) {
            if (estado[i] == NO_LO_VI) {
                int contador = 0;
                dfsElementosConexos(i, contador);
                contadorComponentes.push_back(contador);
            }
        }
    }

    long double Grafo::posibilidadDePerder() {
        // Sumatoria de la parte de arriba
        long double sum = 0;
        long double jugadasTotales = combinatorio(N, 2);
        for (const auto& cantidad : contadorComponentes) {
            sum += combinatorio(cantidad, 2);
        }
        return 1.0 - sum/jugadasTotales;
    }

    void Grafo::verticeConPuente(){
        for (int i = 0; i < N && estado[i] == NO_LO_VI; ++i) {
            cubren(i,-1);
        }
    }

    void Grafo::quitarPuentes(){
        for (const auto &puente : puentes) {
            eliminarElemento(adyacentes[puente.first],puente.second);
            eliminarElemento(adyacentes[puente.second],puente.first);
        }
    }


    void Grafo::resetEstado(){
        for (int i = 0; i < estado.size(); ++i) {
            estado[i] = NO_LO_VI;
        }
    }
    int main() {
        int N, M;
        cin >> N >> M;
        Grafo grafo(N);
        for (int i = 0; i < M; i++) {
            int v, w;
            cin >> v >> w;
            grafo.AgregarArista(v, w);
        }
        grafo.DFS(); // Iniciar el recorrido DFSconTreeYBackEdges desde el nodo 1
        grafo.resetEstado();
        grafo.verticeConPuente();
        grafo.quitarPuentes();
        grafo.contarComponentesConexas();
        cout << fixed <<setprecision(5) <<grafo.posibilidadDePerder()<< endl;//Hago el arreglo que quieren de 5 decimales
        return 0;
    }
