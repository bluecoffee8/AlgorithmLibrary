/** 
 * Author: Kevin Li
 * Lang: C++
 * Description: finds Euler tour
 */

#include <iostream>
#include <vector>
#include <list>
#include <unordered_map>
using namespace std;
#define INF 1000000000
#define pb push_back
#define mp make_pair
#define f first
#define s second
typedef pair<int,int> pi;

struct eulerian {
    struct ph {
        size_t operator () (const pi&a) const {
            return INF*a.first + a.second;
        }
    };
    int n;
    vector<int> *e;
    int *deg;
    list<int> cycle;
    unordered_map<pi,bool,ph> vis;
    eulerian () {}
    eulerian (int _n) : n(_n) {
        e = new vector<int>[n];
        deg = new int[n];
        for (int i = 0; i < n; i++) deg[i] = 0;
        dvis = new bool[n];
    }
    void add(int u, int v) {
        e[u].pb(v);
        e[v].pb(u);
        deg[u]++;
        deg[v]++;
        vis[mp(u,v)] = false;
        vis[mp(v,u)] = false;
    }
    bool *dvis;
    int vc;
    void cdfs(int node) {
        dvis[node] = true;
        vc++;
        for (int i = 0; i < e[node].size(); i++) {
            if (!dvis[e[node][i]]) {
                cdfs(e[node][i]);
            }
        }
    }
    bool connected() {
        vc = 0;
        for (int i = 0; i < n; i++) {
            dvis[i] = false;
        }
        cdfs(0);
        return (vc == n);
    }
    bool exist() {
        if (!connected()) return false;
        for (int i = 0; i < n; i++) {
            if (deg[i]%2) {
                return false;
            }
        }
        return true;
    }
    void dfs(list<int>::iterator itr, int node) {
        for (int i = 0; i < e[node].size(); i++) {
            if (!vis[mp(node,e[node][i])]) {
                vis[mp(node,e[node][i])] = true;
                vis[mp(e[node][i],node)] = true;
                dfs(cycle.insert(itr,node),e[node][i]);
            }
        }
    }
    void driver(int start) {
        dfs(cycle.begin(),start);
    }
    void print() {
        for (auto i = cycle.begin(); i != cycle.end(); i++) {
            cout << *i << " ";
        }
        cout << *(cycle.begin()) << " ";
        cout << endl;
    }
};

int n,m,start;

int main() {
    cin >> n >> m >> start;
    eulerian E = eulerian(n);
    for (int i = 0; i < m; i++) {
        int u,v; cin >> u >> v;
        E.add(u,v);
    }
    if (E.exist()) {
        E.driver(start);
        E.print();
    }
}