/**
 * Author: Kevin Li
 * Lang: C++
 * Description: DSU by Rank implementation
 */

struct dsu {
    int n;
    int *p, *r;
    dsu () {}
    dsu (int _n) : n(_n) {
        p = new int[n];
        r = new int[n];
        preprocess();
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