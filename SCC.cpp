/**
 * Author: Kevin Li
 * Lang: C++
 * Description: finds Strongly Connected Components via Tarjan method, also calculates the kernel DAG
 */

#include <iostream>
#include <vector>
#include <unordered_set>
#include <set>
using namespace std;
#define pb push_back
#define mp make_pair
#define f first
#define s second
typedef pair<int,int> pi;

struct scc {
    int n;
    vector<int> *e;
    int *low, *num;
    vector<int> vst;
    vector<int> *cmp;
    int dfsnum, numscc;
    bool *vis;
    
    int *sccmap;
    
    unordered_set<int> *kdags;
    vector<int> *kdag;
    
    scc () {}
    scc (int _n) : n(_n) {
        e = new vector<int>[n];
        low = new int[n];
        num = new int[n];
        cmp = new vector<int>[n];
        vis = new bool[n];
        numscc = dfsnum = 0;
        sccmap = new int[n];
    }
    void add(int u, int v) {
        e[u].pb(v);
    }
    void dfs(int node) {
        low[node] = num[node] = dfsnum;
        dfsnum++;
        vis[node] = true;
        vst.pb(node);
        for (int i = 0; i < e[node].size(); i++) {
            if (num[e[node][i]] == 0) {
                dfs(e[node][i]);
                low[node] = min(low[node],low[e[node][i]]);
            }
            else if (vis[e[node][i]] == true) {
                low[node] = min(low[node],num[e[node][i]]);
            }
        }
        if (low[node] == num[node]) {
            while (true) {
                int vertex = vst.back();
                vst.pop_back();
                vis[vertex] = false;
                cmp[numscc].pb(vertex);
                if (vertex == node) {
                    break;
                }
            }
            numscc++;
        }
    }
    void driver() {
        numscc = 0;
        dfsnum = 1;
        for (int i = 0; i < n; i++) {
            low[i] = num[i] = 0;
            vis[i] = false;
        }
        for (int i = 0; i < n; i++) {
            if (num[i] == 0) {
                dfs(i);
            }
        }
    }
    
    void kerneldag() {
        kdags = new unordered_set<int>[n];
        kdag = new vector<int>[n];
        for (int i = 0; i < numscc; i++) {
            for (int j = 0; j < cmp[i].size(); j++) {
                sccmap[cmp[i][j]] = i;
            }
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < e[i].size(); j++) {
                if (sccmap[i] != sccmap[e[i][j]]) {
                    kdags[sccmap[i]].insert(sccmap[e[i][j]]);
                }
            }
        }
        for (int i = 0; i < numscc; i++) {
            for (auto j = kdags[i].begin(); j != kdags[i].end(); j++) {
                kdag[i].pb(*j);
            }
        }
    }
    void print() {
        for (int i = 0; i < numscc; i++) {
            for (int j = 0; j < cmp[i].size(); j++) {
                cout << cmp[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
    void printdag() {
        for (int i = 0; i < numscc; i++) {
            for (int j = 0; j < kdag[i].size(); j++) {
                cout << kdag[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
};

int n,m;

int main() {
    cin >> n >> m;
    scc SCC = scc(n);
    for (int i = 0; i < m; i++) {
        int u,v; cin >> u >> v;
        SCC.add(u,v);
    }
    SCC.driver();
    // SCC.kerneldag();
    SCC.print();
    // SCC.printdag();
}