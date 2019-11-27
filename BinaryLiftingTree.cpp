/**
 * Author: Kevin Li
 * Lang: C++
 * Description: Tree with method of binary jumping
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;
#define pb push_back
#define mp make_pair
#define f first
#define s second

struct binaryjumptree {
    int n;
    vector<int> *e;
    int *p;
    int k;
    int **jump;
    
    binaryjumptree () {}
    binaryjumptree (int _n) : n(_n) {
        e = new vector<int>[n];
        p = new int[n];
        k = ceil(log((double)n)/log(2.0));
        jump = new int*[n];
        for (int i = 0; i < n; i++) {
            jump[i] = new int[k+1];
        }
    }
    void add(int u, int v) {
        e[u].pb(v);
        e[v].pb(u);
    }
    void pdfs(int node) {
        for (int i = 0; i < e[node].size(); i++) {
            if (e[node][i] != p[node]) {
                p[e[node][i]] = node;
                pdfs(e[node][i]);
            }
        }
    }
    void driver(int root) {
        p[root] = root; // i think you could also set to -1 if needed
        pdfs(root);
        for (int i = 0; i < n; i++) {
            jump[i][0] = p[i];
        }
        for (int i = 1; i <= k; i++) {
            for (int j = 0; j < n; j++) {
                jump[j][i] = jump[jump[j][i-1]][i-1];
            }
        }
    }
    void print() {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j <= k; j++) {
                cout << jump[i][j] << " ";
            }
            cout << endl;
        }
    }
};

int n, root;

int main() {
    cin >> n >> root;
    binaryjumptree BJT = binaryjumptree(n);
    for (int i = 0; i < n-1; i++) {
        int u,v; cin >> u >> v;
        BJT.add(u,v);
    }
    BJT.driver(root);
    BJT.print();
}