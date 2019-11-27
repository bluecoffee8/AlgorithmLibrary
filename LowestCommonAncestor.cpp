/**
 * Author: Kevin Li
 * Lang: C++
 * Description: finds Lowest Common Ancestor via Binary Jumping method
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

struct lca {
    int n, k;
    vector<int> *e;
    int *p, *d;
    int **jump;
    lca () {}
    lca (int _n) : n(_n) {
        k = (int)floor(log((double)n)/log(2.0));
        e = new vector<int>[n];
        p = new int[n];
        d = new int[n];
        jump = new int*[n];
        for (int i = 0; i < n; i++) {
            jump[i] = new int[k];
        }
    }
    void add(int u, int v) {
        e[u].pb(v);
        e[v].pb(u);
    }
    void pdfs(int node, int dist) {
        d[node] = dist;
        for (int i = 0; i < e[node].size(); i++) {
            if (e[node][i] != p[node]) {
                p[e[node][i]] = node;
                pdfs(e[node][i],dist+1);
            }
        }
    }
    void driver(int root) {
        for (int i = 0; i < n; i++) {
            p[i] = -1;
            d[i] = 0;
        }
        p[root] = root;
        pdfs(root,0);
        for (int i = 0; i < n; i++) {
            jump[i][0] = p[i];
        }
        for (int i = 1; i <= k; i++) {
            for (int j = 0; j < n; j++) {
                jump[j][i] = jump[jump[j][i-1]][i-1];
            }
        }
    }
    int query(int a, int b) {
        if (a == b) return a;
        if (d[a] == d[b]) {
            int exp;
            for (exp = k; exp >= 0; exp--) {
                while (jump[a][exp] == jump[b][exp]) {
                    exp--;
                    if (exp == -1) {
                        break;
                    }
                }
                if (exp == -1) {
                    return p[a];
                } else if (exp == 0) {
                    return jump[a][1];
                }
                a = jump[a][exp];
                b = jump[b][exp];
            }
        } else if (d[a] > d[b]) {
            int temp = a;
            a = b;
            b = temp;
        }
        int exp;
        int mult = 1;
        for (int i = 0; i < k; i++) {
            mult*=2;
        }
        int len = d[b] - d[a];
        for (exp = k; exp >= 0; exp--, mult/=2) {
            while (len < mult) {
                mult/=2;
                exp--;
            }
            len-=mult;
            b = jump[b][exp];
            if (len == 0) {
                break;
            }
        }
        if (a == b) {
            return a;
        }
        for (exp = k; exp >= 0; exp--) {
            while (jump[a][exp] == jump[b][exp]) {
                exp--;
                if (exp == -1) {
                    break;
                }
            }
            if (exp == -1) {
                return p[a];
            } else if (exp == 0) {
                return jump[a][1];
            }
            a = jump[a][exp];
            b = jump[b][exp];
        }
        return 0;
    }
};

int n, root;

int main() {
    cin >> n >> root;
    lca LCA = lca(n);
    for (int i = 0; i < n-1; i++) {
        int u,v; cin >> u >> v;
        LCA.add(u,v);
    }
    LCA.driver(root);
}