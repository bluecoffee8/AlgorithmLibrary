/**
 * Author: Kevin Li
 * Lang: C++
 * Description: Hopcroft-Karp algorithm
 */

#include <iostream>
#include <vector>
#include <queue>
using namespace std;
#define INF 1e9
#define pb push_back

struct hopcroftkarp {
    int nl, nr;
    int n;
    vector<int> *e;
    int *match, *dist;
    int mcbm;
    hopcroftkarp () {}
    hopcroftkarp (int _nl, int _nr) : nl(_nl), nr(_nr) {
        n = nl + nr;
        e = new vector<int>[n+1];
        match = new int[n+1];
        dist = new int[n+1];
        mcbm = 0;
    }
    void add(int u, int v) {
        e[u+1].pb(v+1);
        e[v+1].pb(u+1);
    }
    bool bfs() {
        queue<int> q;
        for (int i = 1; i <= nl; i++) {
            if (match[i] == 0) {
                dist[i] = 0;
                q.push(i);
            } else {
                dist[i] = INF;
            }
        }
        dist[0] = INF;
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            if (dist[v] < dist[0]) {
                for (int i = 0; i < e[v].size(); i++) {
                    if (dist[match[e[v][i]]] == INF) {
                        dist[match[e[v][i]]] = dist[v] + 1;
                        q.push(match[e[v][i]]);
                    }
                }
            }
        }
        if (dist[0] != INF) {
            return true;
        }
        return false;
    }
    bool dfs(int node) {
        if (node != 0) {
            for (int i = 0; i < e[node].size(); i++) {
                if (dist[match[e[node][i]]] == dist[node] + 1) {
                    if (dfs(match[e[node][i]])) {
                        match[e[node][i]] = node;
                        match[node] = e[node][i];
                        return true;
                    }
                }
            }
            dist[node] = INF;
            return false;
        }
        return true;
    }
    void driver() {
        mcbm = 0;
        for (int i = 1; i <= n; i++) {
            match[i] = 0;
        }
        while (bfs()) {
            for (int i = 1; i <= nl; i++) {
                if (match[i] == 0 && dfs(i)) {
                    mcbm++;
                }
            }
        }
    }
    void print() {
        cout << mcbm << endl;
    }
};

int nl, nr, m;

int main() {
    cin >> nl >> nr >> m;
    hopcroftkarp HK = hopcroftkarp(nl,nr);
    for (int i = 0; i < m; i++) {
        int u,v; cin >> u >> v;
        HK.add(u,v);
    }
    HK.driver();
    HK.print();
}