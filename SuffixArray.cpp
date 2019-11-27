/**
 * Author: Kevin Li
 * Lang: C++
 * Description: Suffix Array Implementation, also LCP array in O(n) using Kasai algorithm
 */

#include <iostream>
#include <algorithm>
#include <cmath>
using namespace std;
#define ALPH 52
#define MAXC 256
#define MAXN 200100

struct SA {
    string s;
    int n;
    int *ar, *art, *sa, *sat;
    int *freq;
    int limit;
    
    int *ps;
    int *lcp;
    int *plcp;
    
    SA () {}
    SA (string _s) : s(_s) {
        s += '$';
        n = (int)s.length();
        ar = new int[n];
        art = new int[n];
        sa = new int[n];
        sat = new int[n];
        limit = max(MAXC,n);
        freq = new int[limit];
    }
    void rsort(int j) {
        for (int i = 0; i < limit; i++) freq[i] = 0;
        for (int i = 0; i < n; i++) {
            if (i + j < n) {
                freq[ar[i+j]]++;
            } else {
                freq[0]++;
            }
        }
        int s = 0;
        for (int i = 0; i < limit; i++) {
            int temp = freq[i]; freq[i] = s; s += temp;
        }
        for (int i = 0; i < n; i++) {
            if (sa[i] + j < n) {
                sat[freq[ar[sa[i]+j]]] = sa[i];
                freq[ar[sa[i]+j]]++;
            } else {
                sat[freq[0]] = sa[i];
                freq[0]++;
            }
        }
        for (int i = 0; i < n; i++) sa[i] = sat[i];
    }
    void driver() {
        for (int i = 0; i < n; i++) {
            ar[i] = s[i];
            sa[i] = i;
            art[i] = sat[i] = 0;
        }
        for (int i = 1; i < n; i *= 2) {
            rsort(i); rsort(0);
            art[sa[0]] = 0; int rank = 0;
            for (int j = 1; j < n; j++) {
                if (ar[sa[j]] == ar[sa[j-1]] && ar[sa[j]+i] == ar[sa[j-1]+i]) {
                    art[sa[j]] = rank;
                } else {
                    rank++; art[sa[j]] = rank;
                }
            }
            for (int j = 0; j < n; j++) ar[j] = art[j];
            if (ar[sa[n-1]] == n-1) break;
        }
    }
    
    void lcpk() {
        ps = new int[n];
        lcp = new int[n];
        plcp = new int[n];
        ps[sa[0]] = -1;
        for (int i = 1; i < n; i++) {
            ps[sa[i]] = sa[i-1];
        }
        int match = 0;
        for (int i = 0; i < n; i++) {
            if (ps[i] == -1) {
                plcp[i] = 0;
            } else {
                while (s[i+match] == s[ps[i]+match]) {
                    match++;
                }
                plcp[i] = match;
                match = max(match-1,0);
            }
        }
        for (int i = 0; i < n; i++) {
            lcp[i] = plcp[sa[i]];
        }
    }
    
    void print() {
        for (int i = 1; i <= n-1; i++) {
            cout << sa[i] << " ";
        }
        cout << endl;
    }
    
    void printlcp() {
        for (int i = 2; i <= n-1; i++) {
            cout << lcp[i] << " ";
        }
        cout << endl;
    }
};

string s;

int main() {
    cin >> s;
    SA suffixarray = SA(s);
    suffixarray.driver();
    suffixarray.lcpk();
    suffixarray.print();
    suffixarray.printlcp();
}