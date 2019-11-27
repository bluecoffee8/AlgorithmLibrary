/**
 * Author: Kevin Li
 * Lang: C++
 * Description: nice Dinic's Algorithm implementation
 */

#include <iostream>
#include <vector>
#include <queue>
using namespace std;
#define INF 1e9
#define pb push_back

struct dinic {
    int n;
    int mf;
    vector<int> *e;
    int **res, **flow;
    int *level, *deg;
    dinic () {}
    dinic (int _n) : n(_n) {
        mf = 0;
        res = new int*[n];
        flow = new int*[n];
        e = new vector<int>[n];
        for (int i = 0; i < n; i++) {
            res[i] = new int[n];
            flow[i] = new int[n];
            for (int j = 0; j < n; j++) {
                res[i][j] = 0;
                flow[i][j] = 0;
            }
        }
        level = new int[n];
        deg = new int[n];
        for (int i = 0; i < n; i++) {
            level[i] = deg[i] = 0;
        }
    }
    void add(int u, int v, int w) {
        res[u][v] += w;
        e[u].pb(v);
        e[v].pb(u);
    }
    bool bfs(int source, int sink) {
        for (int i = 0; i < n; i++) level[i] = -1;
        level[source] = 0;
        queue<int> q;
        q.push(source);
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (int i = 0; i < e[v].size(); i++) {
                if (level[e[v][i]] == -1 && flow[v][e[v][i]] < res[v][e[v][i]]) {
                    level[e[v][i]] = level[v] + 1;
                    q.push(e[v][i]);
                }
            }
        }
        if (level[sink] == -1) {
            return false;
        }
        return true;
    }
    int send(int node, int f, int sink) {
        if (node == sink) {
            return f;
        }
        for (; deg[node] < e[node].size(); deg[node]++) {
            if (level[e[node][deg[node]]] == level[node]+1 && flow[node][e[node][deg[node]]] < res[node][e[node][deg[node]]]) {
                int cf = min(f,res[node][e[node][deg[node]]]-flow[node][e[node][deg[node]]]);
                int tf = send(e[node][deg[node]],cf,sink);
                if (tf > 0) {
                    flow[node][e[node][deg[node]]] += tf;
                    flow[e[node][deg[node]]][node] -= tf;
                    return tf;
                }
            }
        }
        return 0;
    }
    void driver(int source, int sink) {
        mf = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                flow[i][j] = 0;
            }
        }
        if (source == sink) {
            return;
        }
        while (bfs(source,sink)) {
            for (int i = 0; i < n; i++) {
                deg[i] = 0;
            }
            while (int f = send(source,INF,sink)) {
                mf += f;
            }
        }
    }
    void print() {
        cout << mf << endl;
    }
};

int n,m,source,sink;

int main() {
    cin >> n >> m >> source >> sink;
    dinic D = dinic(n);
    for (int i = 0; i < m; i++) {
        int u,v,w; cin >> u >> v >> w;
        D.add(u,v,w);
    }
    D.driver(source,sink);
    D.print();
}