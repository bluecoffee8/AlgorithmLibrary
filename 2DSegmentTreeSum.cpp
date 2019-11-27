/**
 * Author: Kevin Li
 * Lang: C++
 * Description: Segment Tree for sums, in two dimensions
 */

#include <iostream>
#include <vector>
using namespace std;

template<class TT>
struct rsq2d {
    template<class T>
    struct rsq {
        struct node {
            T val;
            node *l, *r;
            node () : val(0), l(0), r(0) {}
        };
        node *root = new node();
        int n;
        rsq () {}
        rsq (int _n) : n(_n) {}
        void ext(node *v) {
            if (!v->l) v->l = new node();
            if (!v->r) v->r = new node();
        }
        void pull(node *v) {
            ext(v);
            v->val = v->l->val + v->r->val;
        }
        void bt(node *v, int l, int r, vector<T> a) {
            if (l == r) v->val = a[l];
            else {
                ext(v);
                bt(v->l,l,(l+r)/2,a);
                bt(v->r,(l+r)/2+1,r,a);
                pull(v);
            }
        }
        void bt(vector<T> a) {
            root = new node();
            n = (int)a.size();
            bt(root,0,n-1,a);
        }
        void upd(node *v, int l, int r, int i, T val) {
            if (l == r) v->val += val;
            else {
                ext(v);
                if (i <= (l+r)/2) upd(v->l,l,(l+r)/2,i,val);
                else upd(v->r,(l+r)/2+1,r,i,val);
                pull(v);
            }
        }
        void upd(int i, T val) {
            upd(root,0,n-1,i,val);
        }
        T query(node *v, int l, int r, int L, int R) {
            if (L <= l && r <= R) return v->val;
            else if (L > r || l > R) return 0;
            else {
                ext(v);
                return query(v->l,l,(l+r)/2,L,R) + query(v->r,(l+r)/2+1,r,L,R);
            }
        }
        T query(int L, int R) {
            return query(root,0,n-1,L,R);
        }
    };
    struct node {
        rsq<TT> val;
        node *l, *r;
    };
    node* new_node() {
        node *res= new node(); res->val = rsq<TT>(y);
        res->l = 0; res->r = 0;
        return res;
    }
    int x,y;
    node *root;
    rsq2d () {}
    rsq2d (int _x, int _y) : x(_x), y(_y) {
        root = new_node();
    }
    void ext(node *v) {
        if (!v->l) v->l = new_node();
        if (!v->r) v->r = new_node();
    }
    
    TT **p;
    
    void bx(node *v, int l, int r) {
        vector<TT> t;
        for (int i = 0; i < y; i++) {
            t.push_back(p[r+1][i+1] - p[r+1][i] - p[l][i+1] + p[l][i]);
        }
        v->val.bt(t);
        if (l != r) {
            ext(v);
            bx(v->l,l,(l+r)/2);
            bx(v->r,(l+r)/2+1,r);
        }
    }
    
    void bt(vector<vector<TT>> a) {
        p = new TT*[x+1];
        for (int i = 0; i < x+1; i++) {
            p[i] = new TT[y+1];
            for (int j = 0; j < y+1; j++) {
                p[i][j] = 0;
            }
        }
        for (int i = 1; i < x+1; i++) {
            for (int j = 1; j < y+1; j++) {
                p[i][j] = p[i-1][j] + p[i][j-1] - p[i-1][j-1] + a[i-1][j-1];
            }
        }
        bx(root,0,x-1);
    }
    
    void upd(node *v, int l, int r, int X, int Y, TT val) {
        v->val.upd(Y,val);
        if (l != r) {
            ext(v);
            if (X <= (l+r)/2) {
                upd(v->l,l,(l+r)/2,X,Y,val);
            } else {
                upd(v->r,(l+r)/2+1,r,X,Y,val);
            }
        }
    }
    
    void upd(int X, int Y, TT val) {
        upd(root,0,x-1,X,Y,val);
    }
    
    TT query(node *v, int l, int r, int xl, int xr, int yl, int yr) {
        if (xl <= l && r <= xr) {
            return v->val.query(yl,yr);
        } else if (xl > r || l > xr) {
            return 0;
        } else {
            ext(v);
            return query(v->l,l,(l+r)/2,xl,xr,yl,yr) + query(v->r,(l+r)/2+1,r,xl,xr,yl,yr);
        }
    }
    
    TT query(int xl, int xr, int yl, int yr) {
        return query(root,0,x-1,xl,xr,yl,yr);
    }
};

int n,m,u,q;

int main() {
    cin >> n >> m >> u >> q;
    vector<vector<int>> a;
    for (int i = 0; i < n; i++) {
        a.push_back(vector<int>());
        for (int j = 0; j < m; j++) {
            int val; cin >> val;
            a[i].push_back(val);
        }
    }
    rsq2d<int> RSQ = rsq2d<int>(n,m);
    RSQ.bt(a);
    for (int i = 0; i < u; i++) {
        int x,y,val; cin >> x >> y >> val;
        RSQ.upd(x,y,val);
    }
    int *queries = new int[q];
    for (int i = 0; i < q; i++) {
        int xl,xr,yl,yr; cin >> xl >> xr >> yl >> yr;
        queries[i] = RSQ.query(xl,xr,yl,yr);
    }
    for (int i = 0; i < q; i++) {
        cout << queries[i] << endl;
    }
}