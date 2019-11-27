/**
 * Author: Kevin Li
 * Lang: C++
 * Description: Array with quick range modifications using lazy propagation
 */

#include <iostream>
#include <vector>
using namespace std;

struct lazy_array {
    struct node {
        bool v;
        int val;
        int lazy;
        bool ch;
        int lch;
        node *l, *r;
        node () : v(false), val(0), lazy(0), ch(false), lch(0), l(0), r(0) {}
    };
    typedef node* tn;
    tn root;
    int n;
    lazy_array () {}
    lazy_array (int _n) : n(_n) {
        root = new node();
    }
    void ext(tn v) {
        if (!v->l) v->l = new node();
        if (!v->r) v->r = new node();
    }
    void push(tn v, int l, int r) {
        if (l == r) v->v = true;
        if (v->ch) {
            if (l == r) {
                v->val = v->lch;
            } else {
                ext(v);
                v->l->ch = true;
                v->r->ch = true;
                v->l->lch = v->lch;
                v->r->lch = v->lch;
            }
            v->lazy = 0;
            v->ch = false;
            v->lch = 0;
        } else if (v->lazy != 0) {
            if (l == r) {
                v->val += v->lazy;
            } else {
                ext(v);
                v->l->lazy += v->lazy;
                v->r->lazy += v->lazy;
            }
            v->lazy = 0;
        }
    }
    void build(tn v, int l, int r, vector<int> a) {
        if (l == r) {
            v->v = true;
            v->val = a[l];
        } else {
            ext(v);
            build(v->l,l,(l+r)/2,a);
            build(v->r,(l+r)/2+1,r,a);
        }
    }
    void augment(tn v, int l, int r, int L, int R, int val) {
        push(v,l,r);
        if (l > R || L > r) return;
        if (L <= l && r <= R) {
            if (v->ch) {
                v->lch += val;
            } else {
                v->lazy += val;
            }
        } else {
            ext(v);
            augment(v->l,l,(l+r)/2,L,R,val);
            augment(v->r,(l+r)/2+1,r,L,R,val);
        }
    }
    void change(tn v, int l, int r, int L, int R, int val) {
        push(v,l,r);
        if (l > R || L > r) return;
        if (L <= l && r <= R) {
            v->ch = true;
            v->lch = val;
            v->lazy = 0;
        } else {
            ext(v);
            change(v->l,l,(l+r)/2,L,R,val);
            change(v->r,(l+r)/2+1,r,L,R,val);
        }
    }
    int get(tn v, int l, int r, int i) {
        push(v,l,r);
        if (l == r) return v->val;
        ext(v);
        if (i <= (l+r)/2) {
            return get(v->l,l,(l+r)/2,i);
        } else {
            return get(v->r,(l+r)/2+1,r,i);
        }
    }
};

int main() {
    
}