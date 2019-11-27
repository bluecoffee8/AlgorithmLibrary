/**
 * Author: Kevin Li
 * Lang; C++
 * Description: Monotonic Queue Implementation
 */

#include <iostream>
#include <queue>
#include <deque>
using namespace std;

template<class T>
struct mq {
    #define pb push_back
    #define pf pop_front()
    #define ppb pop_back()
    queue<T> q;
    deque<T> mx, mn;
    mq () {}
    bool empty() {
        return q.empty();
    }
    int size() {
        return q.size();
    }
    T front() {
        return q.front();
    }
    T back() {
        return q.back();
    }
    void push(T val) {
        q.push(val);
        while (!mx.empty()&&mx.back()<val) mx.ppb;
        mx.pb(val);
        while (!mn.empty()&&mn.back()>val) mn.ppb;
        mn.pb(val);
    }
    void pop() {
        if (q.front() == mx.front()) mx.pf;
        if (q.front() == mn.front()) mn.pf;
        q.pop();
    }
    void swap(mq<T> &a, mq<T> &b) {
        swap(a.q,b.q);
        swap(a.mx,b.mx);
        swap(a.mn,b.mn);
    }
    T max() {
        return mx.front();
    }
    T min() {
        return mn.front();
    }
    void print() {
        queue<T> Q = q;
        while (!Q.empty()) {
            cout << Q.front() << " ";
            Q.pop();
        }
        cout << endl;
    }
};

int main() {
    mq<int> MQ = mq<int>();
}