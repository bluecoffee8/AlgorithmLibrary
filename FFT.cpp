/** 
 * Author: Kevin Li
 * Lang: C++
 * Description: Fast Fourier Transform and its applications, also Fast Walsh Hadamard Transform
 */

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <complex>
using namespace std;
#define pb push_back
struct FFT {
    typedef complex<double> cd;
    typedef vector<cd> poly;
    double PI = acos(-1);
    #define pb push_back
    poly P;
    
    FFT () {}
    FFT (poly _P) : P(_P) {}
    void extend() {
        int n = 1;
        while (n < P.size()) n *= 2;
        while (P.size() < n) P.pb(cd(0,0));
    }
    // computes fast fourier transform of polynomial
    void fft(poly &A) {
        int N = (int)A.size();
        if (N == 1) return;
        poly a0(N/2), a1(N/2);
        for (int i = 0; 2*i < N; i++) {
            a0[i] = A[2*i];
            a1[i] = A[2*i+1];
        }
        fft(a0);
        fft(a1);
        double theta = 2*PI/N;
        cd w(1), wp(cos(theta),sin(theta));
        for (int i = 0; 2*i < N; i++) {
            A[i] = a0[i] + w * a1[i];
            A[i+N/2] = a0[i] - w * a1[i];
            w *= wp;
        }
    }
    // computes fast inverse fourier transform of polynomial
    void ifft(poly &A) {
        int N = (int)A.size();
        if (N == 1) return;
        poly a0(N/2), a1(N/2);
        for (int i = 0; 2*i < N; i++) {
            a0[i] = A[2*i];
            a1[i] = A[2*i+1];
        }
        ifft(a0);
        ifft(a1);
        double theta = 2*PI/(-N);
        cd w(1), wp(cos(theta),sin(theta));
        for (int i = 0; 2*i < N; i++) {
            A[i] = a0[i] + w * a1[i];
            A[i+N/2] = a0[i] - w * a1[i];
            A[i] /= 2;
            A[i+N/2] /= 2;
            w *= wp;
        }
    }
    // fast polynomial multiplication
    poly mul(poly &a, poly &b) {
        poly ta(a.begin(),a.end());
        poly tb(b.begin(),b.end());
        int n = 1;
        while (n < a.size() + b.size()) n *= 2;
        while (ta.size() < n) ta.pb(cd(0,0));
        while (tb.size() < n) tb.pb(cd(0,0));
        fft(ta);
        fft(tb);
        for (int i = 0; i < n; i++) {
            ta[i] *= tb[i];
        }
        ifft(ta);
        poly res;
        for (int i = 0; i < n; i++) {
            res.pb(ta[i].real());
        }
        return res;
    }
    // fast polynomial multiplication special case where coefficients are integers
    // then we may have some rounding issue
    typedef vector<int> vi;
    vi mul(vi &a, vi &b) {
        poly ta(a.begin(),a.end());
        poly tb(b.begin(),b.end());
        int n = 1;
        while (n < a.size() + b.size()) n *= 2;
        while (ta.size() < n) ta.pb(cd(0,0));
        while (tb.size() < n) tb.pb(cd(0,0));
        fft(ta);
        fft(tb);
        for (int i = 0; i < n; i++) {
            ta[i] *= tb[i];
        }
        ifft(ta);
        vi res;
        for (int i = 0; i < n; i++) {
            res.pb(round(ta[i].real()));
        }
        return res;
    }
    // karatsuba's algorithm. multiply two large integers, represented by their digits in base 10
    typedef long long ll;
    vi karatsuba(vi &a, vi &b) {
        poly ta(a.begin(),a.end());
        poly tb(b.begin(),b.end());
        int n = 1;
        while (n < a.size() + b.size()) n *= 2;
        while (ta.size() < n) ta.pb(cd(0,0));
        while (tb.size() < n) tb.pb(cd(0,0));
        fft(ta);
        fft(tb);
        for (int i = 0; i < n; i++) {
            ta[i] *= tb[i];
        }
        ifft(ta);
        vi res;
        for (int i = 0; i < n; i++) {
            res.pb(round(ta[i].real()));
        }
        int carry = 0;
        for (int i = 0; i < n; i++) {
            res[i] += carry;
            carry = res[i]/10;
            res[i]%=10;
        }
        return res;
    }
};

// better fft, we do everything in place
// conserves memory

struct BFFT {
    typedef complex<double> cd;
    typedef vector<cd> poly;
    double PI = acos(-1);
    #define pb push_back
    poly P;
    
    BFFT () {}
    BFFT (poly _P) : P(_P) {}
    void extend() {
        int n = 1;
        while (n < P.size()) n *= 2;
        while (P.size() < n) P.pb(cd(0,0));
    }
    int reverse(int n, int l) {
        int res = 0;
        for (int i = 0; i < l; i++) {
            if (n&(1<<i)) {
                res |= (1 << (l-1-i));
            }
        }
        return res;
    }
    void fft(poly &A) {
        int n = (int)A.size();
        int l = 0;
        while ((1<<l) < n) l++;
        for (int i = 0; i < n; i++) {
            if (i < reverse(i,l)) {
                swap(A[i],A[reverse(i,l)]);
            }
        }
        for (int L = 2; L <= n; L *= 2) {
            double theta = 2*PI/L;
            cd wl(cos(theta),sin(theta));
            for (int j = 0; j < n; j += L) {
                cd w(1);
                for (int k = 0; k < L/2; k++) {
                    cd u = A[j+k], v = A[j+k+L/2]*w;
                    A[j+k] = u+v;
                    A[j+k+L/2] = u-v;
                    w *= wl;
                }
            }
        }
    }
    void ifft(poly &A) {
        int n = (int)A.size();
        int l = 0;
        while ((1<<l) < n) l++;
        for (int i = 0; i < n; i++) {
            if (i < reverse(i,l)) {
                swap(A[i],A[reverse(i,l)]);
            }
        }
        for (int L = 2; L <= n; L *= 2) {
            double theta = 2*PI/(-L);
            cd wl(cos(theta),sin(theta));
            for (int j = 0; j < n; j += L) {
                cd w(1);
                for (int k = 0; k < L/2; k++) {
                    cd u = A[j+k], v = A[j+k+L/2]*w;
                    A[j+k] = u+v;
                    A[j+k+L/2] = u-v;
                    w *= wl;
                }
            }
        }
        for (int i = 0; i < n; i++) {
            A[i] /= n;
        }
    }
    poly mul(poly &a, poly &b) {
        poly ta(a.begin(),a.end());
        poly tb(b.begin(),b.end());
        int n = 1;
        while (n < a.size() + b.size()) n *= 2;
        while (ta.size() < n) ta.pb(cd(0,0));
        while (tb.size() < n) tb.pb(cd(0,0));
        fft(ta);
        fft(tb);
        for (int i = 0; i < n; i++) {
            ta[i] *= tb[i];
        }
        ifft(ta);
        poly res;
        for (int i = 0; i < n; i++) {
            res.pb(ta[i].real());
        }
        return res;
    }
    typedef vector<int> vi;
    vi mul(vi &a, vi &b) {
        poly ta(a.begin(),a.end());
        poly tb(b.begin(),b.end());
        int n = 1;
        while (n < a.size() + b.size()) n *= 2;
        while (ta.size() < n) ta.pb(cd(0,0));
        while (tb.size() < n) tb.pb(cd(0,0));
        fft(ta);
        fft(tb);
        for (int i = 0; i < n; i++) {
            ta[i] *= tb[i];
        }
        ifft(ta);
        vi res;
        for (int i = 0; i < n; i++) {
            res.pb(round(ta[i].real()));
        }
        return res;
    }
    typedef long long ll;
    vi karatsuba(vi &a, vi &b) {
        poly ta(a.begin(),a.end());
        poly tb(b.begin(),b.end());
        int n = 1;
        while (n < a.size() + b.size()) n *= 2;
        while (ta.size() < n) ta.pb(cd(0,0));
        while (tb.size() < n) tb.pb(cd(0,0));
        fft(ta);
        fft(tb);
        for (int i = 0; i < n; i++) {
            ta[i] *= tb[i];
        }
        ifft(ta);
        vi res;
        for (int i = 0; i < n; i++) {
            res.pb(round(ta[i].real()));
        }
        int carry = 0;
        for (int i = 0; i < n; i++) {
            res[i] += carry;
            carry = res[i]/10;
            res[i]%=10;
        }
        return res;
    }
};

// even more optimized fft code

struct OFFT {
    typedef complex<double> cd;
    typedef vector<cd> poly;
    double PI = acos(-1);
    #define pb push_back
    poly P;
    
    OFFT () {}
    OFFT (poly _P) : P(_P) {}
    void extend() {
        int n = 1;
        while (n < P.size()) n *= 2;
        while (P.size() < n) P.pb(cd(0,0));
    }
    void fft(poly &a) {
        int n = (int)a.size();
        for (int i = 1, j = 0; i < n; i++) {
            int b = n >> 1;
            for (;j&b;b/=2) {
                j^=b;
            }
            j^=b;
            if (i < j) {
                swap(a[i],a[j]);
            }
        }
        for (int l = 2; l <= n; l *= 2) {
            double theta = 2*PI/l;
            cd wl(cos(theta),sin(theta));
            for (int i = 0; i < n; i += l) {
                cd w(1);
                for (int j = 0; j < l/2; j++) {
                    cd u = a[i+j], v = a[i+j+l/2]*w;
                    a[i+j] = u+v;
                    a[i+j+l/2] = u-v;
                    w *= wl;
                }
            }
        }
    }
    void ifft(poly &a) {
        int n = (int)a.size();
        for (int i = 1, j = 0; i < n; i++) {
            int b = n >> 1;
            for (;j&b;b/=2) {
                j^=b;
            }
            j^=b;
            if (i < j) {
                swap(a[i],a[j]);
            }
        }
        for (int l = 2; l <= n; l *= 2) {
            double theta = 2*PI/(-l);
            cd wl(cos(theta),sin(theta));
            for (int i = 0; i < n; i += l) {
                cd w(1);
                for (int j = 0; j < l/2; j++) {
                    cd u = a[i+j], v = a[i+j+l/2]*w;
                    a[i+j] = u+v;
                    a[i+j+l/2] = u-v;
                    w *= wl;
                }
            }
        }
        for (int i = 0; i < n; i++) {
            a[i] /= n;
        }
    }
    poly mul(poly &a, poly &b) {
        poly ta(a.begin(),a.end());
        poly tb(b.begin(),b.end());
        int n = 1;
        while (n < a.size() + b.size()) n *= 2;
        while (ta.size() < n) ta.pb(cd(0,0));
        while (tb.size() < n) tb.pb(cd(0,0));
        fft(ta);
        fft(tb);
        for (int i = 0; i < n; i++) {
            ta[i] *= tb[i];
        }
        ifft(ta);
        poly res;
        for (int i = 0; i < n; i++) {
            res.pb(ta[i].real());
        }
        return res;
    }
    typedef vector<int> vi;
    vi mul(vi &a, vi &b) {
        poly ta(a.begin(),a.end());
        poly tb(b.begin(),b.end());
        int n = 1;
        while (n < a.size() + b.size()) n *= 2;
        while (ta.size() < n) ta.pb(cd(0,0));
        while (tb.size() < n) tb.pb(cd(0,0));
        fft(ta);
        fft(tb);
        for (int i = 0; i < n; i++) {
            ta[i] *= tb[i];
        }
        ifft(ta);
        vi res;
        for (int i = 0; i < n; i++) {
            res.pb(round(ta[i].real()));
        }
        return res;
    }
    typedef long long ll;
    vi karatsuba(vi &a, vi &b) {
        poly ta(a.begin(),a.end());
        poly tb(b.begin(),b.end());
        int n = 1;
        while (n < a.size() + b.size()) n *= 2;
        while (ta.size() < n) ta.pb(cd(0,0));
        while (tb.size() < n) tb.pb(cd(0,0));
        fft(ta);
        fft(tb);
        for (int i = 0; i < n; i++) {
            ta[i] *= tb[i];
        }
        ifft(ta);
        vi res;
        for (int i = 0; i < n; i++) {
            res.pb(round(ta[i].real()));
        }
        int carry = 0;
        for (int i = 0; i < n; i++) {
            res[i] += carry;
            carry = res[i]/10;
            res[i]%=10;
        }
        return res;
    }
};

struct FWHT {
    typedef vector<double> vd;
    typedef long long ll;
    typedef vector<ll> vll;
    
    vd fwht(vd a) {
        for (int l = 1; 2*l <= a.size(); l *= 2) {
            for (int i = 0; i < a.size(); i += 2*l) {
                for (int j = 0; j < l; j++) {
                    double u = a[i+j];
                    double v = a[i+l+j];
                    a[i+j] = u+v;
                    a[i+l+j] = u-v;
                }
            }
        }
        return a;
    }
    
    vd rev(vd &a) {
        vd res = fwht(a);
        for (int i = 0; i < res.size(); i++) {
            res[i] /= a.size();
        }
        return res;
    }
    
    vd conv(vd a, vd b) {
        int n = 1;
        while (n < a.size() + b.size()) n *= 2;
        while (a.size() < n) a.push_back(0);
        while (b.size() < n) b.push_back(0);
        a = fwht(a); b = fwht(b);
        for (int i = 0; i < n; i++) {
            a[i] *= b[i];
        }
        a = rev(a);
        return a;
    }
    
    vll conv(vll a, vll b) {
        vd A, B;
        for (ll i : a) A.push_back(i);
        for (ll i : b) B.push_back(i);
        vd c = conv(A,B);
        vll C; for (double i : c) C.push_back(round(i));
        return C;
    }
};

int main() {
    srand(time(0));
    OFFT fft = OFFT();
    vector<int> a,b;
    for (int i = 0; i < 2*1e5; i++) {
        a.pb(0);
        b.pb(0);
    }
    fft.mul(a,b);
}