/**
 * Author: Kevin Li
 * Lang: C++
 * Description: determines primality of first n integer via Erastothenes Sieve
 */

#include <iostream>
using namespace std;

struct sieve {
    int n;
    bool *prime;
    sieve () {}
    sieve (int _n) : n(_n) {
        prime = new bool[n+1];
        prime[0] = false;
        prime[1] = false;
        for (int i = 2; i < n+1; i++) {
            prime[i] = true;
        }
    }
    void driver(int _n) {
        for (int i = 2; i < _n+1; i++) {
            if (prime[i]) {
                for (int j = 2*i; j < _n+1; j += i) {
                    prime[j] = false;
                }
            }
        }
    }
    void print(int _n) {
        for (int i = 0; i < _n+1; i++) {
            if (prime[i]) {
                cout << "1";
            } else {
                cout << "0";
            }
        }
        cout << endl;
    }
};

int a,b,c;

int main() {
    cin >> a >> b >> c;
    sieve S = sieve(a);
    S.driver(b);
    S.print(c);
}