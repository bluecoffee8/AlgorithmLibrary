/** 
 * Author: Kevin Li
 * Lang: C++
 * Description: segment tree for range sum
 */

#include <iostream>
#include <vector>
using namespace std;

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

int n,u,q;
vector<int> a;

int main() {
    cin >> n >> u >> q;
    for (int i = 0; i < n; i++) {
        int val; cin >> val; a.push_back(val);
    }
    rsq<int> RSQ = rsq<int>(n);
    RSQ.bt(a);
    for (int i = 0; i < u; i++) {
        int j, val; cin >> j >> val;
        RSQ.upd(j,val);
    }
    int *queries = new int[q];
    for (int i = 0; i < q; i++) {
        int l,r; cin >> l >> r;
        queries[i] = RSQ.query(l,r);
    }
    for (int i = 0; i < q; i++) {
        cout << queries[i] << endl;
    }
}