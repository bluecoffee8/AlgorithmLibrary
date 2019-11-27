/**
 * Author: Kevin Li
 * Lang: C++
 * Description: finds Minimum Spanning Tree via Kruskal Algorithm
 */

#include <iostream>
#include <queue>
using namespace std;
#define mp make_pair
#define pb push_back
#define f first
#define s second
#define INF 1e9
typedef pair<int,int> pi;

struct kruskal {
    struct dsu {
        int n;
        int *p, *r;
        dsu () {}
        dsu (int _n) : n(_n) {
            p = new int[n];
            r = new int[n];
        }
        void preprocess() {
            for (int i = 0; i < n; i++) {
                p[i] = i;
                r[i] = 0;
            }
        }
        int find(int i) {
            if (i != p[i]) p[i] = find(p[i]);
            return p[i];
        }
        void merge(int i, int j) {
            int ic = find(i);
            int jc = find(j);
            if (r[ic] < r[jc]) {
                p[ic] = jc;
            } else if (r[ic] > r[jc]) {
                p[jc] = ic;
            } else {
                p[jc] = ic;
                r[ic]++;
            }
        }
    };
    int n;
    dsu d;
    priority_queue<pair<int,pi>> pq;
    int nc;
    int mst;
    kruskal () {}
    kruskal (int _n) : n(_n) {
        d = dsu(n);
        nc = n;
        mst = 0;
    }
    void add(int u, int v, int w) {
        pq.push(mp(-w,mp(u,v)));
    }
    void driver() {
        mst = 0;
        nc = n;
        d.preprocess();
        while (nc != 1 && !pq.empty()) {
            int u = pq.top().s.f;
            int v = pq.top().s.s;
            int w = -pq.top().f;
            pq.pop();
            int fu = d.find(u);
            int fv = d.find(v);
            if (fu != fv) {
                mst += w;
                d.merge(fu,fv);
                nc--;
            }
        }
        while (!pq.empty()) {
            pq.pop();
        }
    }
    void print() {
        if (nc == 1) {
            cout << mst << endl;
        } else {
            cout << INF << endl;
        }
    }
};

int n,m;

int main() {
    cin >> n >> m;
    kruskal K = kruskal(n);
    for (int i = 0; i < m; i++) {
        int u,v,w; cin >> u >> v >> w;
        K.add(u,v,w);
    }
    K.driver();
    K.print();
}