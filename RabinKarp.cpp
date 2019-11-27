/** 
 * Author: Kevin Li
 * Lang: C++
 * Description: Rabin-Karp String Search Algorithm (uses pair of hashes to minimize collision)
 */

#include <iostream>
#include <vector>
using namespace std;
#define M 1000000000193
#define x1 100003
#define x2 100019
typedef long long ll;
typedef pair<ll,ll> pll;
#define pb push_back
#define mp make_pair
#define fi first
#define se second

struct rkhash {
    string s;
    int n;
    pll pow;
    pll pattern;
    
    vector<int> indices;
    
    rkhash() {}
    rkhash (string _s) : s(_s) {
        n = (int)s.length();
    }
    void process() {
        pow = mp(1,1);
        pattern = mp(0,0);
        for (int i = 0; i < n-1; i++) {
            pow.fi = (pow.fi*x1)%M;
            pow.se = (pow.se*x2)%M;
        }
        for (int i = 0; i < n; i++) {
            pattern.fi = (x1*pattern.fi+s[i])%M;
            pattern.se = (x2*pattern.se+s[i])%M;
        }
    }
    void search(string S) {
        while (!indices.empty()) indices.pop_back();
        int m = (int)S.length();
        if (m < n) return;
        pll text = mp(0,0);
        for (int i = 0; i < n; i++) {
            text.fi = (x1*text.fi+S[i])%M;
            text.se = (x2*text.se+S[i])%M;
        }
        for (int i = 0; i <= m-n; i++) {
            if (pattern.fi == text.fi && pattern.se == text.se) {
                bool flag = true;
                for (int j = 0; j < n; j++) {
                    if (s[j] != S[i+j]) {
                        flag = false;
                        break;
                    }
                }
                if (flag) {
                    indices.pb(i);
                }
            }
            if (i < m-n) {
                text.fi = (x1*(text.fi-S[i]*pow.fi)+S[i+n])%M;
                text.se = (x2*(text.se-S[i]*pow.se)+S[i+n])%M;
                if (text.fi < 0) text.fi += M;
                if (text.se < 0) text.se += M;
            }
        }
    }
    void print() {
        for (int i = 0; i < indices.size(); i++) {
            cout << indices[i] << " ";
        }
        cout << endl;
    }
};

string s, S;

int main() {
    cin >> s >> S;
    rkhash RKH = rkhash(s);
    RKH.process();
    RKH.search(S);
    RKH.print();
}