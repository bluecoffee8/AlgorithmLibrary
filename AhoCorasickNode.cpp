/**
 * Author: Kevin Li
 * Lang: C++
 * Description: Aho Corasick implementation with objects and pointers
 */

#include <iostream>
#include <queue>
#include <vector>
using namespace std;
#define ALPH 52

struct aho {
    int val(char c) {
        if (c >= 'a') return c-'a';
        else return c-'A'+26;
    }
    struct node {
        node *fail; // fail link
        node *dict; // dictionary link, see "Aho-Corasick Dictionary Links"
        node *edge[ALPH]; // edge
        vector<int> words;
        node () : fail(0), dict(0) { // default constructor
            for (int i = 0; i < ALPH; i++) edge[i] = 0;
        }
    };
    node *root = 0; // root
    node **loc; // array for storing the location of keys
    string *keys; // actual keys
    int n = 0; // dictionary size
    aho () {}
    aho (int _n) : n(_n) {
        loc = new node*[n];
        keys = new string[n];
    }
    node* insert(node *&i, string s, int j, int idx) {
        if (i == NULL) i = new node();
        if (j == s.length()) {
            i->words.push_back(idx);
            return i;
        }
        return insert(i->edge[val(s[j])],s,j+1,idx);
    }
    void insert(int i, string s) { // i is index of key
        loc[i] = insert(root,s,0,i);
        keys[i] = s;
    }
    void automaton() {
        root->fail = root->dict = root;
        queue<node*> q;
        for (int i = 0; i < ALPH; i++) {
            if (root->edge[i] != NULL) {
                node *j = root->edge[i];
                j->fail = j->dict = root;
                q.push(j);
            }
        }
        while (!q.empty()) {
            node* cur = q.front(); q.pop();
            for (int i = 0; i < ALPH; i++) {
                if (cur->edge[i] != 0) {
                    node *j = cur->edge[i];
                    node *k = cur->fail;
                    while (k != root && k->edge[i] == 0) k = k->fail;
                    if (k->edge[i] != 0) k = k->edge[i];
                    j->fail = k;
                    if (k->words.size() != 0) j->dict = k;
                    else j->dict = k->dict;
                    q.push(j);
                }
            }
        }
    }
    node* nxt(node *i, int c) {
        node *j = i;
        while (j->edge[c] == 0 && j != root) {
            j = j->fail;
        }
        if (j->edge[c] != 0) return j->edge[c];
        else return j;
    }
    vector<int> *found;
    void search(string S) {
        found = new vector<int>[n];
        node *cur = root;
        for (int i = 0; i < (int)S.length(); i++) {
            cur = nxt(cur,val(S[i]));
            for (int j = 0; j < cur->words.size(); j++) {
                found[cur->words[j]].push_back(i-(int)keys[cur->words[j]].length()+1);
            }
            node *temp = cur;
            while (temp != root) {
                temp = temp->fail;
                for (int j = 0; j < temp->words.size(); j++) {
                    found[temp->words[j]].push_back(i-(int)keys[temp->words[j]].length()+1);
                }
            }
        }
    }
    // below is alternative method of search which may be slightly faster
    // uses the dictionary links instead of suffix links
    // this is faster since we are really only interested in states where there is at least one word occurrence
    void search2(string S) {
        found = new vector<int>[n];
        node *cur = root;
        for (int i = 0; i < (int)S.length(); i++) {
            cur = nxt(cur,val(S[i]));
            for (int j = 0; j < cur->words.size(); j++) {
                found[cur->words[j]].push_back(i-(int)keys[cur->words[j]].length()+1);
            }
            node *temp = cur;
            while (temp != root) {
                temp = temp->dict;
                for (int j = 0; j < temp->words.size(); j++) {
                    found[temp->words[j]].push_back(i-(int)keys[temp->words[j]].length()+1);
                }
            }
        }
    }
    void print() {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < found[i].size(); j++) {
                cout << found[i][j] << " ";
            }
            cout << endl;
        }
    }
    // debug by dfs traversal of aho-corasick tree
    void dfs(node *i) {
        if (i == NULL) return;
        for (int j = 0; j < i->words.size(); j++) {
            cout << i->words[j] << " ";
        }
        cout << endl;
        for (int j = 0; j < ALPH; j++) {
            if (i->edge[j] != NULL) cout << j << " ";
            dfs(i->edge[j]);
        }
    }
    void dfs_print() {
        dfs(root);
    }
};

int n;
string S;
aho AHO;

int main() {
    cin >> n >> S;
    AHO = aho(n);
    for (int i = 0; i < n; i++) {
        string s; cin >> s;
        AHO.insert(i,s);
    }
    AHO.automaton();
    AHO.search2(S);
    AHO.print();
    // AHO.dfs_print();
}