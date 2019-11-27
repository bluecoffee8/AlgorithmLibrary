/**
 * Author: Kevin Li
 * Lang: C++
 * Description: Dijkstra's shortest path algorithm
 */

#include <iostream>
#include <queue>
#include <vector>

using namespace std;

#define MAXN 100000
#define INF -100000000
#define pb push_back
#define mp make_pair
#define f first
#define s second

typedef pair<int,int> pii;

struct Dijkstra {
    int nv;
    vector<pii> *e;
    bool *vis;
    priority_queue<pii> dist;
    int source;
    int *distances;
    Dijkstra () {
        nv = 0;
        source = 0;
        e = nullptr;
        vis = nullptr;
        distances = nullptr;
    }
    Dijkstra (int _nv, int _source) {
        nv = _nv;
        source = _source;
        e = new vector<pii>[nv];
        vis = new bool[nv];
        distances = new int[nv];
        
        for (int i = 0; i < nv; i++) {
            distances[i] = INF;
            vis[i] = false;
        }
    }
    void addEdge(int u, int v, int w) {
        e[u].pb(mp(-w,v));
    }
    void driver() {
        dist.push(mp(0,source));
        while (!dist.empty()) {
            int d = dist.top().f;
            int v = dist.top().s;
            dist.pop();
            if (!vis[v]) {
                distances[v] = max(distances[v],d);
                vis[v] = true;
                for (int i = 0; i < e[v].size(); i++) {
                    dist.push(mp(d+e[v][i].f,e[v][i].s));
                }
            }
        }
    }
    void print() {
        for (int i = 0; i < nv; i++) {
            cout << -distances[i] << " ";
        }
        cout << endl;
    }
};

int nv, ne, source;

int main() {
    cin >> nv >> ne >> source;
    Dijkstra D = Dijkstra(nv,source);
    for (int i = 0; i < ne; i++) {
        int u,v,w; cin >> u >> v >> w;
        D.addEdge(u,v,w);
    }
    D.driver();
    D.print();
}