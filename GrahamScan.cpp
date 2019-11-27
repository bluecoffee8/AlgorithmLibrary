/**
 * Author: Kevin Li
 * Lang: C++
 * Description: Convex Hull via Graham Scan
 */

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;
#define MAXN 100000000
#define INF 1000000000
#define EPS 1e-9
bool eq(double a, double b) {
    return (abs(a-b)<EPS);
}
struct point {
    double x,y;
    point () : x(0), y(0) {}
    point(double _x, double _y) : x(_x), y(_y) {}
};
bool eqp(point a, point b) {
    return (eq(a.x,b.x)&&eq(a.y,b.y));
}
bool collinear(point a, point b, point c) {
    if ((eq(a.x,b.x)&&eq(a.y,b.y))||(eq(a.x,c.x)&&eq(a.y,c.y))) {
        return true;
    }
    double s1 = (b.y-a.y)/(b.x-a.x);
    double s2 = (c.y-a.y)/(c.x-a.x);
    return eq(s1,s2);
}
double ed(point a, point b) {
    return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}
struct polygon {
    int n;
    vector<point> vert; // remember n = vert.size()
    polygon () : n(0) {}
};
double cross(point a, point b) {
    return a.x*b.y-a.y*b.x;
}
bool ccw(point a, point o, point b) {
    point t1, t2;
    t1.x = a.x-o.x;
    t1.y = a.y-o.y;
    t2.x = o.x-b.x;
    t2.y = o.y-b.y;
    return cross(t1,t2)>0.0;
}
point pivot;
bool cmp(point a, point b) {
    if (collinear(pivot,a,b)) {
        return ed(pivot,a)<ed(pivot,b);
    }
    return (atan2(a.y-pivot.y,a.x-pivot.x)-atan2(b.y-pivot.y,b.x-pivot.x))<0;
}
struct grahamscan {
    int n;
    polygon P;
    vector<point> hull;
    grahamscan(int _n) : n(_n) {
        P.n = _n;
    }
    void init() {
        for (int i = 0; i < n; i++) {
            double x,y; cin >> x >> y;
            point temp(x,y);
            P.vert.push_back(temp);
        }
    }
    void driver() {
        if (n <= 3) {
            for (int i = 0; i < n; i++) {
                hull.push_back(P.vert[i]);
            }
            if (!eqp(P.vert[0],P.vert[n-1])) {
                hull.push_back(P.vert[0]);
            }
            return;
        }
        int pidx = 0;
        for (int i = 1; i < n; i++) {
            if (P.vert[i].y < P.vert[pidx].y || (P.vert[i].y == P.vert[pidx].y && P.vert[i].x > P.vert[pidx].x)) {
                pidx = i;
            }
        }
        point temp = P.vert[0];
        P.vert[0] = P.vert[pidx];
        P.vert[pidx] = temp;
        pivot = P.vert[0];
        sort(++P.vert.begin(),P.vert.end(),cmp);
        hull.push_back(P.vert[n-1]);
        hull.push_back(P.vert[0]);
        hull.push_back(P.vert[1]);
        int i = 2;
        while (i < n) {
            int j = (int)hull.size()-1;
            if (ccw(hull[j-1],hull[j],P.vert[i])) {
                hull.push_back(P.vert[i]);
                i++;
            } else {
                hull.pop_back();
            }
        }
        hull.pop_back();
    }
    void print() {
        for (int i = 0; i < hull.size(); i++) {
            cout << hull[i].x << " " << hull[i].y << endl;
        }
    }
};

int n;

int main() {
    cin >> n;
    grahamscan GS = grahamscan(n);
    GS.init();
    GS.driver();
    GS.print();
}