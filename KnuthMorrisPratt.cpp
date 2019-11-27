/**
 * Author: Kevin Li
 * Lang: C++
 * Description: classic KMP String search
 */

#include <iostream>
#include <vector>
using namespace std;
typedef long long ll;
#define pb push_back

struct kmp {
    string s;
    int *p;
    
    vector<int> indices;
    
    kmp () {}
    kmp (string _s) : s(_s) {}

    void process() {
        p = new int[s.length()];
        int i = 0, j = -1; p[0] = -1;
        for (int i = 1; i < s.length(); i++) p[i] = 0;
        while (i < s.length()) {
            while (j >= 0 && s[i] != s[j]) {
                j = p[j];
            }
            i++; j++;
            p[i] = j;
        }
    }
    
    void search(string S) {
        while (!indices.empty()) indices.pop_back();
        int i = 0, j = 0;
        while (i < S.length()) {
            while (j >= 0 && S[i] != s[j]) {
                j = p[j];
            }
            i++; j++;
            if (j == s.length()) {
                indices.pb(i-j);
                j = p[j];
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
    kmp KMP = kmp(s);
    KMP.process();
    KMP.search(S);
    KMP.print();
}