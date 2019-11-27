/**
 * Author: Kevin Li
 * Lang: C++
 * Description: segment tree for min/max queries
 */

#include <iostream>
#include <vector>
using namespace std;
#define INF 1e9 // change as needed

template<class T>
struct strmq {
    #define mp make_pair
    #define f first
    #define s second
    struct node {
        T maxv, minv;
        T lazy;
        bool ch;
        T cv;
        node *left, *right;
        node () : maxv(-INF), minv(INF), lazy(0), ch(false), cv(0), left(0), right(0) {}
    };
    node *root = 0;
    vector<T> a;
    int n;
    
    strmq() {
        root = new node();
        a.clear();
        n = 0;
    }
    
    void push(node *v, int l, int r) {
        if (v->ch) {
            if (l != r) {
                if (v->left == 0) v->left = new node();
                if (v->right == 0) v->right = new node();
                v->left->ch = true;
                v->right->ch = true;
                v->left->lazy = 0;
                v->right->lazy = 0;
                v->left->cv = v->cv;
                v->right->cv = v->cv;
            }
            v->lazy = 0;
            v->ch = false;
            v->maxv = v->cv;
            v->minv = v->cv;
            v->cv = 0;
        } else if (v->lazy != 0) {
            if (l != r) {
                if (v->left == 0) v->left = new node();
                if (v->right == 0) v->right = new node();
                if (v->left->ch) {
                    v->left->cv += v->lazy;
                } else {
                    v->left->lazy += v->lazy;
                }
                if (v->right->ch) {
                    v->right->cv += v->lazy;
                } else {
                    v->right->lazy += v->lazy;
                }
            }
            v->maxv += v->lazy;
            v->minv += v->lazy;
            v->lazy = 0;
        }
    }
    void pull(node *v, int l, int r) {
        if (v->left == 0) v->left = new node();
        if (v->right == 0) v->right = new node();
        push(v->left,l,(l+r)/2);
        push(v->right,(l+r)/2+1,r);
        v->maxv = max(v->left->maxv,v->right->maxv);
        v->minv = min(v->left->minv,v->right->minv);
    }
    void bt(node *v, int l, int r) {
        if (l == r) {
            v->minv = v->maxv = a[l];
        } else {
            if (v->left == 0) v->left = new node();
            if (v->right == 0) v->right = new node();
            bt(v->left,l,(l+r)/2);
            bt(v->right,(l+r)/2+1,r);
            pull(v,l,r);
        }
    }
    void construct(vector<T> A) {
        a = A;
        n = (int)a.size();
        root = new node(0,n-1);
        bt(0,0,n-1);
    }
    void update(node *v, int l, int r, int i, T val) {
        push(v,l,r);
        if (l == r) {
            v->minv += val;
            v->maxv += val;
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
            if (v->ch) {
                v->cv += val;
            } else {
                v->lazy += val;
            }
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
    void change(node *v, int l, int r, int L, int R, T val) {
        push(v,l,r);
        if (L <= l && r <= R) {
            v->ch = true;
            v->lazy = 0;
            v->cv = val;
        } else if (l > R || L > r) {
            return;
        } else {
            if (v->left == 0) v->left = new node();
            if (v->right == 0) v->right = new node();
            change(v->left,l,(l+r)/2,L,R,val);
            change(v->right,(l+r)/2+1,r,L,R,val);
            pull(v,l,r);
        }
    }
    void cd(int L, int R, T val) {
        change(root,0,n-1,L,R,val);
    }
    T minquery(node *v, int l, int r, int L, int R) {
        push(v,l,r);
        if (L <= l && r <= R) {
            return v->minv;
        } else if (l > R || L > r) {
            return INF;
        } else {
            return min(((v->left==0)?INF:minquery(v->left,l,(l+r)/2,L,R)),((v->right==0)?INF:minquery(v->right,(l+r)/2+1,r,L,R)));
        }
    }
    T minqd(int L, int R) {
        return minquery(root,0,n-1,L,R);
    }
    T maxquery(node *v, int l, int r, int L, int R) {
        push(v,l,r);
        if (L <= l && r <= R) {
            return v->maxv;
        } else if (l > R || L > r) {
            return -INF;
        } else {
            return max(((v->left==0)?-INF:maxquery(v->left,l,(l+r)/2,L,R)),((v->right==0)?-INF:maxquery(v->right,(l+r)/2+1,r,L,R)));
        }
    }
    T maxqd(int L, int R) {
        return maxquery(root,0,n-1,L,R);
    }
    T getVal(int i) {
        return maxqd(i,i);
    }
};