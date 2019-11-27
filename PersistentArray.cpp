/**
 * Author: Kevin Li
 * Lang: C++
 * Description: Persistent Array Data Structure
 */

#include <iostream>
using namespace std;

template <class T>
struct seg_array {
    struct node {
        T val;
        node *l, *r;
        node () : val(0), l(0), r(0) {}
        node (T _val) : val(_val), l(0), r(0) {}
    };
    
    // notice that for each possible persistent array, we must separately keep track of the size of array
    // we can do this by setting the last value of our array to be the size of the array
    
    // ext is O(1)
    void ext(node *v) {
        if (!v->l) v->l = new node();
        if (!v->r) v->r = new node();
    }
    // build is O(n)
    void build(node *v, int l, int r) {
        if (l == r) {
            v->val = 0;
        } else {
            ext(v);
            build(v->l,l,(l+r)/2);
            build(v->r,(l+r)/2+1,r);
        }
    }
    // get_val is O(lg n)
    T get_val(node *v, int l, int r, int i) {
        if (l == r) {
            return v->val;
        } else {
            if (i <= (l+r)/2) {
                return get_val(v->l,l,(l+r)/2,i);
            } else {
                return get_val(v->r,(l+r)/2+1,r,i);
            }
        }
    }
    // set_val is O(lg n)
    void set_val(node *v, int l, int r, int i, T val) {
        if (l == r) {
            v->val = val;
        } else {
            if (i <= (l+r)/2) {
                set_val(v->l,l,(l+r)/2,i,val);
            } else {
                set_val(v->r,(l+r)/2+1,r,i,val);
            }
        }
    }
    // copy is a O(lg n) function
    // we have arrays a and b of equal size. assuming that root of array b is newly initialized to new node()
    // we wish to copy subarray [L,R] of a --> b, while remaining values of b are default set to 0
    // or they are uninitialized. in total this is O(lg n) because [L,R] is decomposed into O(lg n) pieces
    void copy(node *a, node *b, int l, int r, int L, int R) {
        if (l > R || L > r) return;
        if (L <= l && r <= R) {
            b = a; // notice that we don't really create a new copy, but we set a pointer. so if a changes, b changes as well
        } else {
            // we might also want to check that a->l, a->r are well-defined here too
            ext(b);
            copy(a->l,b->l,l,(l+r)/2,L,R);
            copy(a->r,b->r,(l+r)/2+1,r,L,R);
        }
    }
};

// while I'm at it might as well write a lazy version where you can set an entire range to some value

template<class T>
struct lazy_seg_array {
    struct node {
        T val, lazy;
        bool ch;
        node *l, *r;
        node () : val(0), l(0), r(0), ch(false) {}
        node (T _val) : val(_val), l(0), r(0), ch(false) {}
    };
    void ext(node *v) {
        if (!v->l) v->l = new node();
        if (!v->r) v->r = new node();
    }
    void push(node *v, int l, int r) {
        if (v->ch) {
            v->ch = false;
            if (l == r) {
                v->val = v->lazy;
            } else {
                ext(v);
                v->l->ch = true;
                v->r->ch = true;
                v->l->lazy = v->lazy;
                v->r->lazy = v->lazy;
            }
        }
    }
    void build(node *v, int l, int r) {
        if (l == r) {
            v->val = 0;
        } else {
            ext(v);
            build(v->l,l,(l+r)/2);
            build(v->r,(l+r)/2+1,r);
        }
    }
    T get_val(node *v, int l, int r, int i) {
        push(v,l,r);
        if (l == r) {
            return v->val;
        } else {
            if (i <= (l+r)/2) {
                return get_val(v->l,l,(l+r)/2,i);
            } else {
                return get_val(v->r,(l+r)/2+1,r,i);
            }
        }
    }
    void set_val(node *v, int l, int r, int L, int R, T val) {
        push(v,l,r);
        if (l > R || L > r) return;
        if (L <= l && r <= R) {
            v->ch = true;
            v->lazy = val;
        } else {
            ext(v);
            set_val(v->l,l,(l+r)/2,L,R,val);
            set_val(v->r,(l+r)/2+1,r,L,R,val);
        }
    }
    void copy(node *a, node *b, int l, int r, int L, int R) {
        if (l > R || L > r) return;
        if (L <= l && r <= R) {
            b = a; // notice that we don't really create a new copy, but we set a pointer. so if a changes, b changes as well
        } else {
            // we might also want to check that a->l, a->r are well-defined here too
            ext(b);
            copy(a->l,b->l,l,(l+r)/2,L,R);
            copy(a->r,b->r,(l+r)/2+1,r,L,R);
        }
    }
};

int main() {
    
}