#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <set>
#include <limits.h>

using namespace std;

// Функция DFS для обхода графа и перечисления всех вершин
void DFS(const vector<vector<int>>& mat, int start) {
    vector<int> used(mat.size(), 0); // массив для отслеживания посещенных вершин
    stack<int> Stack;
    Stack.push(start);
    std::cout << "DFS traversal: ";
    
    while (!Stack.empty()) {
        int node = Stack.top();
        Stack.pop();

        if (used[node] == 2) continue;
        
        std::cout << node << " "; // вывод текущей вершины
        used[node] = 2; // отмечаем как посещенную

        // Добавляем смежные вершины
        for (int j = 0; j < mat.size(); j++) {
            if (mat[node][j] > 0 && used[j] != 2) {
                Stack.push(j);
                used[j] = 1; // отмечаем вершину как обнаруженную
            }
        }
    }
    std::cout << endl;
}

// Функция для поиска кратчайшего пути от вершины start до всех остальных (алгоритм Дейкстры)
vector<int> dijkstra(const vector<vector<int>>& mat, int start) {
    int n = mat.size();
    vector<int> dist(n, INT_MAX); // расстояние от start до всех вершин
    dist[start] = 0;
    set<pair<int, int>> pq; // set для хранения расстояний и узлов
    pq.insert({0, start});
    while (!pq.empty()) {
        std::cout << "test: ";
        std::cout << pq.begin()->first << " " << pq.begin()->second << std::endl;
        int node = pq.begin()->second;
        pq.erase(pq.begin());
        for (int j = 0; j < n; j++) {
            if (mat[node][j] > 0) { // если существует ребро
                int newDist = dist[node] + mat[node][j];
                if (newDist < dist[j]) {
                    pq.erase({dist[j], j});
                    dist[j] = newDist;
                    pq.insert({newDist, j});
                }
            }
        }
    }

    // Вывод кратчайших путей
    std::cout << "Shortest paths from vertex " << start << ": ";
    for (int i = 0; i < n; i++) {
        if (dist[i] == INT_MAX) std::cout << "inf ";
        else std::cout << dist[i] << " ";
    }
    std::cout << endl;

    return dist;
}

// Функция для подсчета степени вершин дерева и средней степени
void calculateDegree(const vector<vector<int>>& tree) {
    int n = tree.size();
    vector<int> degree(n, 0);

    // BFS для обхода дерева
    queue<int> Queue;
    vector<int> visited(n, 0);
    Queue.push(0);
    visited[0] = 1;

    while (!Queue.empty()) {
        int node = Queue.front();
        Queue.pop();

        for (int j = 0; j < n; j++) {
            if (tree[node][j] > 0 && !visited[j]) {
                degree[node]++;
                degree[j]++;
                Queue.push(j);
                visited[j] = 1;
            }
        }
    }

    // Вывод степени каждой вершины и средней степени
    int sumDegree = 0;
    std::cout << "Vertex degrees: ";
    for (int d : degree) {
        std::cout << d << " ";
        sumDegree += d;
    }
    std::cout << endl;
    std::cout << "Average degree: " << (double)sumDegree / n << endl;
}

// Функция для построения минимального остова (алгоритм Прима)
vector<vector<int>> primMST(const vector<vector<int>>& mat) {
    int n = mat.size();
    vector<int> key(n, INT_MAX); // вес ребер для включения в MST
    vector<bool> inMST(n, false); // включены ли вершины в MST
    vector<vector<int>> mst(n, vector<int>(n, 0)); // матрица для хранения MST

    key[0] = 0; // начнем с вершины 0
    int u;
for (int count = 0; count < n - 1; count++) {
    u = -1;

    // Найти вершину с минимальным значением key, которая не входит в MST
    for (int v = 0; v < n; v++) {
        u = (!inMST[v] && (u == -1 || key[v] < key[u])) ? v : u;
    }//заменеиить на тернарную операцию

        inMST[u] = true;

        // Обновить значение key и MST для смежных вершин
        for (int v = 0; v < n; v++) {
            if (mat[u][v] && !inMST[v] && mat[u][v] < key[v]) {
                key[v] = mat[u][v];
                mst[u][v] = mst[v][u] = mat[u][v];
            }
        }
    }

    std::cout << "MST adjacency matrix:" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            std::cout << mst[i][j] << " ";
        }
        std::cout << endl;
    }

    return mst;
}

int main() {
    // Матрица смежности
    vector<vector<int>> mat = {
        { 0, 9, 0, 2, 7, 4, 7, 8, 4 },
        { 9, 0, 5, 5, 0, 7, 8, 1, 5 },
        { 0, 5, 0, 6, 3, 9, 3, 3, 0 },
        { 2, 5, 6, 0, 3, 4, 3, 9, 5 },
        { 7, 0, 3, 3, 0, 0, 9, 0, 4 },
        { 4, 7, 9, 4, 0, 0, 5, 9, 6 },
        { 7, 8, 3, 3, 9, 5, 0, 9, 8 },
        { 8, 1, 3, 9, 0, 9, 9, 0, 1 },
        { 4, 5, 0, 5, 4, 6, 8, 1, 0 }
    };

    // 1. Запуск DFS для перечисления всех вершин
    DFS(mat, 0);

    // 2. Поиск кратчайшего пути от вершины 0 до всех остальных
    dijkstra(mat, 0);

    // 4. Построение минимального остова алгоритмом Прима
    vector<vector<int>> mst = primMST(mat);

    // 3. Подсчет степени вершин в минимальном остове и средней степени
    calculateDegree(mst);

    return 0;
}