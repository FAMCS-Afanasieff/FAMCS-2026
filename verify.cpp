#include <cstdio>
#include <complex>
#include <vector>
#include <cmath>
#include <set>
#include <string>
#include <numeric>
using namespace std;
typedef complex<double> C;
const double PI = acos(-1.0);

int mobiusOK(long a,long b,long c,long d,int N){
    for(int n=1;n<=N;n++){
        for(int j=0;j<n;j++){
            C xi = exp(C(0,1)*(2.0*PI*j/n));
            C den = C(c,0)*xi + C(d,0);
            if(abs(den) < 1e-9) return 0;
            C val = (C(a,0)*xi + C(b,0)) / den;
            C p = pow(val, n);
            if(abs(p - C(1,0)) > 1e-6) return 0;
        }
    }
    return 1;
}
