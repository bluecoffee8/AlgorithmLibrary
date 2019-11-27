/**
 * Author: Kevin Li
 * Lang: C++
 * Description: Centroid decomposition on a tree
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <unordered_map>
using namespace std;
#define MAXN 1000000
#define MAXH 30

struct centroid_decomposition {
    typedef long long ll;
    typedef pair<int,int> pii;
    #define pb push_back
    #define mp make_pair
    #define f first
    #define s second
    
    int n,h;
    vector<int> *e;
    
    vector<int> *ctree;
    int *cp;
    int *layer;
    int maxl;
    int *stsize;
    int croot;
    int ss;
    int **st;
    bool **viss;
    bool **vist;
    bool *cvis;
    bool *cpvis;
    
    int **anc;
    
    centroid_decomposition (int _n) : n(_n) {
        e = new vector<int>[n];
        ctree = new vector<int>[n];
        ss = 0;
        h = ceil((double)(log(double(n))/log(2.0)));
        st = new int*[h];
        viss = new bool*[h];
        vist = new bool*[h];
        cvis = new bool[n];
        cpvis = new bool[n];
        cp = new int[n];
        layer = new int[n];
        stsize = new int[n];
        maxl = 0;
        for (int i = 0; i < n; i++) {
            cvis[i] = false;
            cpvis[i] = false;
            cp[i] = 0;
            layer[i] = 0;
            stsize[i] = 0;
        }
        croot = 0;
        for (int i = 0; i < h; i++) {
            st[i] = new int[n];
            viss[i] = new bool[n];
            vist[i] = new bool[n];
            for (int j = 0; j < n; j++) {
                st[i][j] = 0;
                viss[i][j] = false;
                vist[i][j] = false;
            }
        }
    }
    void add(int u, int v) {
        e[u].pb(v);
        e[v].pb(u);
    }
    void sdfs(int node, int depth) {
        viss[depth][node] = true;
        ss++;
        st[depth][node] = 1;
        for (int i = 0; i < e[node].size(); i++) {
            if (!viss[depth][e[node][i]] && !cvis[e[node][i]]) {
                sdfs(e[node][i],depth);
                st[depth][node]+=st[depth][e[node][i]];
            }
        }
    }
    int centroid(int root, int depth) {
        bool c = true;
        vist[depth][root] = true;
        int heavy = -1;
        for (int i = 0; i < e[root].size(); i++) {
            if (!vist[depth][e[root][i]] && !cvis[e[root][i]]) {
                if (st[depth][e[root][i]] > ss/2) {
                    c = false;
                }
                if (heavy == -1 || st[depth][e[root][i]]>st[depth][heavy]) {
                    heavy = e[root][i];
                }
            }
        }
        if (c && ss-st[depth][root]<=ss/2) {
            return root;
        }
        return centroid(heavy,depth);
    }
    int decompose(int root, int depth) {
        ss = 0;
        sdfs(root,depth);
        int c = centroid(root,depth);
        cvis[c] = true;
        stsize[c] = ss;
        return c;
    }
    int centroiddecomposition(int root, int depth) {
        int c = decompose(root,depth);
        for (int i = 0; i < e[c].size(); i++) {
            if (!cvis[e[c][i]]) {
                int cs = centroiddecomposition(e[c][i],depth+1);
                ctree[c].pb(cs);
                ctree[cs].pb(c);
            }
        }
        return c;
    }
    
    void cparentdfs(int node, int depth) {
        cpvis[node] = true;
        layer[node] = depth;
        maxl = max(maxl,depth);
        for (int i = 0; i < ctree[node].size(); i++) {
            if (!cpvis[ctree[node][i]]) {
                cp[ctree[node][i]] = node;
                cparentdfs(ctree[node][i],depth+1);
            }
        }
    }
    void cparent() {
        cp[croot] = -1;
        cparentdfs(croot,0);
    }
    
    
    
    
    
    void driver() {
        croot = centroiddecomposition(0,0);
        cparent();
    }
    
    void ancd() {
        anc = new int*[h];
        for (int i = 0; i < h; i++) {
            anc[i] = new int[n];
            for (int j = 0; j < n; j++) {
                anc[i][j] = -1;
            }
        }
        for (int i = 0; i < n; i++) {
            int cur = layer[i];
            int ver = i;
            while (cur >= 0) {
                anc[cur][i] = ver;
                ver = cp[ver];
                cur--;
            }
        }
    }
};

int main() {
    
}