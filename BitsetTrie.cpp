/**
 * Author: Kevin Li
 * Lang: C++
 * Description: Trie data structure with bits instead of regular characters
 */

#include <iostream>
using namespace std;

struct bitset_trie {
    #define MAXN 1e7
    #define INF 1e9
    #define BIT 62
    typedef long long ll;
    int **t, s = 0;
    int *sz;
    int n;
    bitset_trie (int _n) : n(_n) {
        t = new int*[n];
        sz = new int[n];
        for (int i = 0; i < n; i++) {
            t[i] = new int[2]; t[i][0] = t[i][1] = 0; sz[i] = 0;
        }
    }
    void init() {
        for (int i = 0; i < n; i++) {
            t[i][0] = t[i][1] = 0;
        }
    }
    void insert(ll x, int m) {
        int c = 0;
        sz[c] += m;
        for (int i = 0; i < BIT; i++) {
            int nxt = (x & (1LL << i)) >> i;
            if (!t[c][nxt]) {
                s++;
                t[c][nxt] = s;
            }
            c = t[c][nxt];
            sz[c] += m;
        }
    }
    // given x, computes maximum xor of x w/ any element in the trie
    // larger bits are more significant and located closer to root
    // so by the way xor works we always try to get a bit that
    // doesn't correspond to the bit in x
    ll max_xor(ll x) {
        if (sz[0] == 0) {
            return -INF;
        }
        int c = 0;
        for (int i = 0; i < BIT; i++) {
            int nxt = ((x & (1LL << i)) >> i) ^ 1;
            if (!t[c][nxt] || !sz[t[c][nxt]]) {
                nxt ^= 1;
            }
            c = t[c][nxt];
            if (nxt) {
                x ^= (1LL << i);
            }
        }
        return x;
    }
};

int main() {
    
}