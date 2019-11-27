/**
 * Author: Kevin Li
 * Lang: C++
 * Description: Sparse Table
 */

#include <iostream>
#include <cmath>
using namespace std;

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
    int query(int l, int r) {
        int k = (int)floor(log((double)r-l+1)/log(2.0));
        if (a[t[l][k]] < a[t[r-(1<<k)+1][k]]) {
            return t[l][k];
        } else {
            return t[r-(1<<k)+1][k];
        }
    }
};

int main() {
    
}