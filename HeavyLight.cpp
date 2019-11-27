/**
 * Author: Kevin Li
 * Lang: C++
 * Description: Heavy Light Decomposition on a tree, answers path queries for both edges and vertices, also uses novel approach to answer subtree queries by rearranging visit order
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

template<class TT>
struct hld {
    #define pb push_back
    #define mp make_pair
    #define f first
    #define s second
    typedef pair<int,TT> pt;
    struct lca {
        struct spt {
            int n, *a, **t, h;
            spt () {}
            spt (int _n) : n(_n) {
                a = new int[n];
                h = (int)ceil(log((double)n)/log(2.0));
                t = new int*[n];
                for (int i = 0; i < n; i++) {
                    t[i] = new int[h];
                }
            }
            void init() {
                for (int i = 0; i < n; i++) {
                    cin >> a[i];
                }
            }
            void driver() {
                for (int i = 0; i < n; i++) {
                    t[i][0] = i;
                }
                for (int i = 0; i < n; i++) {
                    for (int i = 1; (1 << i) <= n; i++) {
                        for (int j = 0; j+(1<<i) < n+1; j++) {
                            if (a[t[j][i-1]] < a[t[j+(1<<(i-1))][i-1]]) {
                                t[j][i] = t[j][i-1];
                            } else {
                                t[j][i] = t[j+(1<<(i-1))][i-1];
                            }
                        }
                    }
                }
            }
            int query(int l, int r) {
                int k = (int)floor(log((double)r-l+1)/log(2.0));
                if (a[t[l][k]] < a[t[r-(1<<k)+1][k]]) {
                    return t[l][k];
                } else {
                    return t[r-(1<<k)+1][k];
                }
            }
        };
        int n;
        vector<int> *e;
        bool *vis;
        int *p, *euler, *depth, *f;
        int idx;
        spt table;
        lca () {}
        lca (int _n) : n(_n) {
            e = new vector<int>[n];
            vis = new bool[n];
            p = new int[n];
            euler = new int[2*n];
            depth = new int [2*n];
            f = new int[2*n];
            table = spt(2*n);
        }
        void add(int u, int v) {
            e[u].pb(v);
            e[v].pb(u);
        }
        void pdfs(int node) {
            vis[node] = true;
            for (int i = 0; i < e[node].size(); i++) {
                if (!vis[e[node][i]]) {
                    p[e[node][i]] = node;
                    pdfs(e[node][i]);
                }
            }
        }
        void ldfs(int node, int d) {
            f[node] = idx;
            euler[idx] = node;
            depth[idx] = d;
            idx++;
            for (int i = 0; i < e[node].size(); i++) {
                if (e[node][i] != p[node]) {
                    ldfs(e[node][i],d+1);
                    euler[idx] = node;
                    depth[idx] = d;
                    idx++;
                }
            }
        }
        void driver(int root) {
            idx = 0;
            for (int i = 0; i < n; i++) {
                vis[i] = false;
                p[i] = -1;
                euler[2*i] = 0;
                euler[2*i+1] = 0;
                depth[2*i] = 0;
                depth[2*i+1] = 0;
                f[2*i] = 0;
                f[2*i+1] = 0;
            }
            pdfs(root);
            ldfs(root,0);
            for (int i = 0; i < 2*n; i++) {
                table.a[i] = depth[i];
            }
            table.driver();
        }
        int query(int a, int b) {
            return euler[table.query(min(f[a],f[b]),max(f[a],f[b]))];
        }
    };
    template<class T>
    struct stsum {
        struct node {
            T val;
            T lazy;
            node *left, *right;
            node () : val(0), lazy(0), left(0), right(0) {}
        };
        node *root = 0;
        vector<T> a;
        int n;
        
        stsum () {
            root = new node();
            a.clear();
            n = 0;
        }
        
        void push(node *v, int l, int r) {
            if (v->lazy != 0) {
                if (l != r) {
                    if (v->left == 0) v->left = new node();
                    if (v->right == 0) v->right = new node();
                    v->left->lazy += v->lazy;
                    v->right->lazy += v->lazy;
                }
                v->val += (v->lazy)*(r-l+1);
                v->lazy = 0;
            }
        }
        void pull(node *v, int l, int r) {
            if (v->left == 0) v->left = new node();
            if (v->right == 0) v->right = new node();
            push(v->left,l,(l+r)/2);
            push(v->right,(l+r)/2+1,r);
            v->val = v->left->val + v->right->val;
        }
        void bt(node *v, int l, int r) {
            if (l == r) {
                v->val = a[l];
            } else {
                if (v->left == 0) v->left = new node();
                if (v->right == 0) v->right = new node();
                bt(v->left,l,(l+r)/2);
                bt(v->right,(l+r)/2+1,r);
                pull(v,l,r);
            }
        }
        void update(node *v, int l, int r, int i, T val) {
            push(v,l,r);
            if (l == r) {
                v->val += val;
            } else {
                if (i <= (l+r)/2) {
                    if (v->left == 0) v->left = new node();
                    update(v->left,l,(l+r)/2,i,val);
                } else {
                    if (v->right == 0) v->right = new node();
                    update(v->right,(l+r)/2+1,r,i,val);
                }
                pull(v,l,r);
            }
        }
        void ud(int i, T val) {
            update(root,0,n-1,i,val);
        }
        void augment(node *v, int l, int r, int L, int R, T val) {
            push(v,l,r);
            if (L <= l && r <= R) {
                v->lazy += val;
            } else if (l > R || L > r) {
                return;
            } else {
                if (v->left == 0) v->left = new node();
                if (v->right == 0) v->right = new node();
                augment(v->left,l,(l+r)/2,L,R,val);
                augment(v->right,(l+r)/2+1,r,L,R,val);
                pull(v,l,r);
            }
        }
        void ad(int L, int R, T val) {
            augment(root,0,n-1,L,R,val);
        }
        T query(node *v, int l, int r, int L, int R) {
            push(v,l,r);
            if (L <= l && r <= R) {
                return v->val;
            } else if (l > R || L > r) {
                return 0;
            } else {
                return ((v->left==0)?0:query(v->left,l,(l+r)/2,L,R))+((v->right==0)?0:query(v->right,(l+r)/2+1,r,L,R));
            }
        }
        
        T qd(int L, int R) {
            return query(root,0,n-1,L,R);
        }
        void construct(vector<T> A) {
            a = A;
            root = new node();
            n = (int)a.size();
            bt(root,0,n-1);
        }
        
        T getVal(int i) {
            return qd(i,i);
        }
    };
    
    
    
    // hld code
    
    int n; // vertices
    vector<pt> *e; // adj list
    
    lca LCA; // lca subroutine
    
    int *p, *d, *h; // parent, depth, heavy
    
    bool *le; // if edge is light edge or not
    
    TT *weight; // vertex weights. one time usage at beginning only
    
    int *r; // root of chain for vertex i
    
    int *idxv; // roots/indices for vertex chains
    int *idxe; // roots/indices for edge chains
    
    vector<pair<int,TT>> av; // vector for vertices
    vector<pair<pair<int,int>,TT>> ae; // vector for edges
    // vectors above are one time usage because they are inserted into segment tree and range modifications
    vector<TT> a; // all purpose vector
    
    stsum<TT> edges = stsum<TT>(); // edge chains
    stsum<TT> vertices = stsum<TT>(); // vertex chains
    
    int *stsz; // substree size of tree rooted at i, including node
    
    hld () {}
    hld (int _n) : n(_n) { // constructor
        e = new vector<pt>[n];
    }
    
    void add(int u, int v, TT w) { // add edge
        e[u].pb(mp(v,w));
        e[v].pb(mp(u,w));
    }
    
    void lca_driver(int root) { // lca subroutine
        LCA = lca(n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < e[i].size(); j++) {
                LCA.e[i].pb(e[i][j].first);
            }
        }
        LCA.driver(root);
    }
    
    
    // dfs for heavy light
    
    int dfs(int node) {
        int sz = 1, sub = 0;
        for (int i = 0; i < e[node].size(); i++) {
            if (e[node][i].f != p[node]) {
                p[e[node][i].f] = node;
                d[e[node][i].f] = d[node] + 1;
                int subsz = dfs(e[node][i].f);
                if (subsz > sub) {
                    h[node] = e[node][i].f;
                    sub = subsz;
                    swap(e[node][i],e[node][0]);
                }
                sz += subsz;
            }
        }
        stsz[node] = sz;
        return sz;
    }
    
    // driver
    
    void driver(int root) {
        p = new int[n];
        d = new int[n];
        h = new int[n];
        le = new bool[n];
        r = new int[n];
        idxv = new int[n];
        idxe = new int[n];
        stsz = new int[n];
        weight = new TT[n];
        for (int i = 0; i < n; i++) {
            p[i] = -1; d[i] = -1; h[i] = -1;
            le[i] = false; r[i] = idxv[i] = idxe[i] = -1;
            stsz[i] = 0;
            weight[i] = 0;
        }
        dfs(root);
        for (int i = 0; i < n; i++) {
            if (p[i] == -1 || h[p[i]] != i) {
                r[i] = i;
                if (p[i] != -1) le[i] = true;
                for (int j = h[i]; j != -1; j = h[j]) {
                    r[j] = i;
                }
            }
        }
    }
    
    void add_val(int i, TT val) {
        weight[i] = val;
    }
    
    // dfs for the final construction
    
    void Dfs(int node) {
        av.pb(mp(node,weight[node]));
        for (int i = 0; i < e[node].size(); i++) {
            if (e[node][i].f != p[node]) {
                ae.pb(mp(mp(node,e[node][i].f),e[node][i].s));
                Dfs(e[node][i].f);
            }
        }
    }
    
    // final construction algorithm
    
    void construct(int root) {
        Dfs(root);
        a.clear();
        for (int i = 0; i < av.size(); i++) {
            a.pb(av[i].s);
            idxv[av[i].f] = i;
        }
        vertices.construct(a);
        a.clear();
        for (int i = 0; i < ae.size(); i++) {
            a.pb(ae[i].s);
            idxe[ae[i].f.s] = i;
        }
        edges.construct(a);
    }
    
    // update edges on path
    
    void upde(int u, int v, TT val) {
        while (r[u] != r[v]) {
            if (d[r[u]] > d[r[v]]) swap(u,v);
            if (le[v]) {
                edges.ud(idxe[v],val);
                v = p[v];
            } else {
                edges.ad(idxe[r[v]]+1,idxe[v],val);
                v = r[v];
            }
        }
        if (d[u] > d[v]) swap(u,v);
        if (u != v) edges.ad(idxe[u]+1,idxe[v],val);
    }
    
    // update vertices on path
    
    void updv(int u, int v, TT val) {
        while (r[u] != r[v]) {
            if (d[r[u]] > d[r[v]]) swap(u,v);
            vertices.ad(idxv[r[v]],idxv[v],val);
            v = p[r[v]];
        }
        if (d[u] > d[v]) swap(u,v);
        vertices.ad(idxv[u],idxv[v],val);
    }
    
    // update both edges/vertices on path
    
    void upd(int u, int v, TT val) {
        upde(u,v,val);
        updv(u,v,val);
    }
    
    // updates a subtree's edges
    
    void usde(int u, TT val) {
        if (stsz[u] == 1) return;
        edges.ad(idxe[u]+1,idxe[u]+stsz[u]-1,val);
    }
    
    // updates a subtree's vertices
    
    void usdv(int u, TT val) {
        vertices.ad(idxv[u],idxv[u]+stsz[u]-1,val);
    }
    
    // updates both subtree edge/vertices
    
    void usd(int u, TT val) {
        usde(u,val);
        usdv(u,val);
    }
    
    // gets sum of values on edge path
    
    TT qpde(int u, int v) {
        TT res = 0;
        while (r[u] != r[v]) {
            if (d[r[u]] > d[r[v]]) swap(u,v);
            if (le[v]) {
                res += edges.getVal(idxe[v]);
                v = p[v];
            } else {
                res += edges.qd(idxe[r[v]]+1,idxe[v]);
                v = r[v];
            }
        }
        if (d[u] > d[v]) swap(u,v);
        if (u != v) res += edges.qd(idxe[u]+1,idxe[v]);
        return res;
    }
    
    // gets sum of values on vertex path
    
    TT qpdv(int u, int v) {
        TT res = 0;
        while (r[u] != r[v]) {
            if (d[r[u]] > d[r[v]]) swap(u,v);
            res += vertices.qd(idxv[r[v]],idxv[v]);
            v = p[r[v]];
        }
        if (d[u] > d[v]) swap(u,v);
        res += vertices.qd(idxv[u],idxv[v]);
        return res;
    }
    
    // gets sum of values on edge/vertex path
    
    TT qpd(int u, int v) {
        return qpde(u,v) + qpdv(u,v);
    }
    
    // gets sum of edges in subtree
    
    TT qsde(int u) {
        if (stsz[u] == 1) return 0;
        return edges.qd(idxe[u]+1,idxe[u]+stsz[u]-1);
    }
    
    // gets sum of vertices in subtree
    
    TT qsdv(int u) {
        return vertices.qd(idxv[u],idxv[u]+stsz[u]-1);
    }
    
    // gets sum of both edges and vertices in subtree
    
    TT qsd(int u) {
        return qsde(u) + qsdv(u);
    }
};

int main() {
    // construction should be:
    // initiate n, root, hld
    // add edges
    // driver(root)
    // add vertex weights using add_val
    // final construct()
    int n, root; cin >> n >> root;
    hld<int> HLD = hld<int>(n);
    for (int i = 0; i < n-1; i++) {
        int u,v,w; cin >> u >> v >> w;
        HLD.add(u,v,w);
    }
    HLD.driver(root);
    for (int i = 0; i < n; i++) {
        int val; cin >> val;
        HLD.add_val(i,val);
    }
    HLD.construct(root);
}