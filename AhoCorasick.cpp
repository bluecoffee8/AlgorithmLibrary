/**
 * Author: Kevin Li
 * Lang: C++
 * Description: Aho-Corasick String Search Algorithm
 */

#include <iostream>
#include <vector>
#include <queue>
using namespace std;
typedef long long ll;
typedef pair<int,int> pii;
typedef vector<int> vi;
#define pb push_back
#define ALPH 52 // default stuff

int val(char c) { // we're assuming most problems involve only like lower case letters and maybe uppercase. if this is not the case then we need to modify this function
    if (c >= 'a') {
        return c-'a';
    } else {
        return c-'A'+26;
    }
}

struct aho {
    int alph;
    int **trie;
    int state;
    int totlen;
    int *f;
    vector<string> keys;
    vi *word;
    vi *found;
    
    aho () {}
    aho (int _alph) : alph(_alph) {
        totlen = 0;
    }
    void add(string s) {
        keys.pb(s);
        totlen += s.length();
    }
    void insert(string s, int idx) {
        int cur = 0;
        for (int i = 0; i < s.length(); i++) {
            int index = val(s[i]);
            if (trie[cur][index] == -1) {
                trie[cur][index] = state;
                state++;
            }
            
            cur = trie[cur][index];
        }
        word[cur].push_back(idx);
    }
    int nxtst(int cur, int c) {
        int st = cur;
        while (trie[st][c] == -1) {
            st = f[st];
        }
        return trie[st][c];
    }
    void automaton() {
        state = 1;
        trie = new int*[totlen+1];
        word = new vi[totlen+1];
        f = new int[totlen+1];
        for (int i = 0; i <= totlen; i++) {
            f[i] = 0;
            trie[i] = new int[alph];
            for (int j = 0; j < alph; j++) {
                trie[i][j] = -1;
            }
        }
        for (int i = 0; i < keys.size(); i++) {
            insert(keys[i],i);
        }
        for (int i = 0; i < alph; i++) {
            if (trie[0][i] == -1) {
                trie[0][i] = 0;
            }
        }
        queue<int> q;
        for (int i = 0; i < alph; i++) {
            if (trie[0][i] != 0) {
                f[trie[0][i]] = 0;
                q.push(trie[0][i]);
            }
        }
        while (!q.empty()) {
            int st = q.front();
            q.pop();
            for (int i = 0; i < alph; i++) {
                if (trie[st][i] != -1) {
                    int fail = f[st];
                    while (trie[fail][i] == -1) {
                        fail = f[fail];
                    }
                    fail = trie[fail][i];
                    f[trie[st][i]] = fail;
                    q.push(trie[st][i]);
                }
            }
        }
    }
    void search(string S) {
        found = new vi[keys.size()];
        int cur = 0;
        for (int i = 0; i < S.length(); i++) {
            cur = nxtst(cur,val(S[i]));
            for (int j = 0; j < word[cur].size(); j++) {
                found[word[cur][j]].pb(i-(int)keys[word[cur][j]].length()+1);
            }
            int temp = cur;
            while (temp) {
                temp = f[temp];
                for (int j = 0; j < word[temp].size(); j++) {
                    found[word[temp][j]].pb(i-(int)keys[word[temp][j]].length()+1);
                }
            }
        }
    }
    void print() {
        for (int i = 0; i < keys.size(); i++) {
            for (int j = 0; j < found[i].size(); j++) {
                cout << found[i][j] << " ";
            }
            cout << endl;
        }
    }
};

int n;
string S;

int main() {
    cin >> n >> S;
    aho AHO = aho(ALPH);
    for (int i = 0; i < n; i++) {
        string s; cin >> s;
        AHO.add(s);
    }
    AHO.automaton();
    AHO.search(S);
    AHO.print();
}