/**
 * Author: Kevin Li
 * Lang: C++
 * Description: Binary Indexed Tree for range sum, or Fenwick tree
 */

#include <iostream>
#include <vector>
using namespace std;
#define pb push_back

template<class T>
struct bit {
    int n;
    vector<T> b;
    
    bit () {}
    bit (int _n) : n(_n) {
        b.clear();
        for (int i = 0; i < n+1; i++) b.pb(0);
    }
    
    void adjust(int index, T value) {
        for (int i = index; i <= n; i += ((i) & (-i))) {
            b[i] += value;
        }
    }
    
    T rsq(int l, int r) {
        T sl = 0, sr = 0;
        for (; l; l -= ((l)&(-l))) {
            sl += b[l];
        }
        for (; r; r -= ((r)&(-r))) {
            sr += b[r];
        }
        return sr - sl;
    }
    
    void construct(vector<T> a) {
        n = (int)a.size();
        b.clear();
        for (int i = 0; i < n+1; i++) b.pb(0);
        for (int i = 1; i <= n; i++) {
            adjust(i,a[i-1]);
        }
    }
};

int n;
vector<int> a;

int main() {
    cin >> n;
    for (int i = 0; i < n; i++) {
        int v; cin >> v;
        a.pb(v);
    }
    bit<int> BIT = bit<int>();
    BIT.construct(a);
}