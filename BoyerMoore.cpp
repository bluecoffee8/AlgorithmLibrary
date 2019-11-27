/**
 * Author: Kevin Li
 * Lang: C++
 * Description: Boyer Moore string search algorithm
 */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdio>
using namespace std;
#define ALPH 256
#define pb push_back

struct boyermoore {
    int alph;
    string s;
    int *bad;
    
    vector<int> indices;
    
    boyermoore () {}
    boyermoore (string _s, int _alph) : s(_s), alph(_alph) {
        bad = new int[alph];
    }
    
    void process() {
        for (int i = 0; i < alph; i++) {
            bad[i] = -1;
        }
        for (int i = 0; i < s.length(); i++) {
            bad[(int)s[i]] = i;
        }
    }
    
    void search(string S) {
        while (!indices.empty()) indices.pop_back();
        int shift = 0;
        while (shift <= (S.length()-s.length())) {
            int i = s.length()-1;
            while (i >= 0 && s[i] == S[shift+i]) {
                i--;
            }
            if (i < 0) {
                indices.pb(shift);
                if (shift + s.length() < S.length()) {
                    shift += s.length() - bad[S[shift+s.length()]];
                } else {
                    shift += 1;
                }
            } else {
                shift += max(1,i-bad[S[shift+i]]);
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

string S,s;

int main() {
    cin >> s >> S;
    boyermoore BM = boyermoore(s,ALPH);
    BM.process();
    BM.search(S);
    BM.print();
}