/**
 * Author: Kevin Li
 * Lang: C++
 * Description: Topologically Sorts DAG
 */

#include <iostream>
#include <vector>
#include <queue>
using namespace std;
#define MAXN 1000000

struct TopSort {
    int n;
    vector<int> *e;
    int *degrees;
    queue<int> topNodes;
    vector<int> topologicalOrdering;
    bool acyclic;
    TopSort(int _n) : n(_n) {
        e = new vector<int>[n];
        degrees = new int[n];
        acyclic = true;
        for (int i = 0; i < n; i++) {
            degrees[i] = 0;
        }
    }
    void add(int u, int v) {
        e[u].push_back(v);
        degrees[v]++;
    }
    void checkAcyclic() {
        if (topologicalOrdering.size() != n) {
            acyclic = false;
        }
    }
    void driver() {
        for (int i = 0; i < n; i++) {
            if (degrees[i] == 0) {
                topNodes.push(i);
            }
        }
        while (!topNodes.empty()) {
            int topNode = topNodes.front();
            topNodes.pop();
            topologicalOrdering.push_back(topNode);
            for (int i = 0; i < e[topNode].size(); i++) {
                degrees[e[topNode][i]]--;
                if (degrees[e[topNode][i]] == 0) {
                    topNodes.push(e[topNode][i]);
                }
            }
        }
        checkAcyclic();
    }
    void print() {
        for (int i = 0; i < topologicalOrdering.size(); i++) {
            cout << topologicalOrdering[i] << " ";
        }
        cout << endl;
    }
};

int n,m;

int main() {
    cin >> n >> m;
    TopSort TS = TopSort(n);
    for (int i = 0; i < m; i++) {
        int u,v; cin >> u >> v;
        TS.add(u,v);
    }
    TS.driver();
    TS.print();
}