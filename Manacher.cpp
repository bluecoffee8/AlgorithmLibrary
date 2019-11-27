/**
 * Author: Kevin Li
 * Lang: C++
 * Description: Manacher Palindrome search algorithm
 */

#include <iostream>
#include <vector>
using namespace std;

struct manacher {
    string S;
    manacher (string _s) : S(_s) {}
    vector<int> res;
    void driver() {
        res.clear();
        string s = "@";
        for (char c : S) {
            s += c;
            s += "#";
        }
        s[s.length()-1] = '&';
        res = vector<int>(s.length()-1);
        int l,h; l = h = 0;
        for (int i = 1; i <= s.length()-1; i++) {
            if (i != 1) {
                res[i] = min(h-i,res[h-i+l]);
            }
            while (s[i-res[i]-1] == s[i+res[i]+1]) {
                res[i]++;
            }
            if (i + res[i] > h) {
                l = i - res[i];
                h = i + res[i];
            }
        }
        res.erase(res.begin());
        for (int i = 0; i < res.size(); i++) {
            if ((i&1) == (res[i]&1)) {
                res[i]++;
            }
        }
    }
    void print() {
        for (int i = 0; i < res.size(); i++) {
            cout << res[i] << " ";
        }
        cout << endl;
    }
};

string s;

int main() {
    cin >> s;
    manacher M = manacher(s);
    M.driver();
    M.print();
}