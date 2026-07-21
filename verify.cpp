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

string classifyMobius(long a,long b,long c,long d){
    auto ev=[&](double x){ return (a*x+b)/(c*x+d); };
    double f2=ev(2), f3=ev(3);
    if(fabs(f2-2)<1e-9 && fabs(f3-3)<1e-9) return "x";
    if(fabs(f2-0.5)<1e-9 && fabs(f3-1.0/3)<1e-9) return "1/x";
    char buf[64]; snprintf(buf,64,"(%ldx+%ld)/(%ldx+%ld)",a,b,c,d); return buf;
}

int monomialOK(int k,int N){
    for(int n=1;n<=N;n++) for(int j=0;j<n;j++){
        C xi = exp(C(0,1)*(2.0*PI*j/n));
        if(k<0 && abs(xi)<1e-12) return 0;
        C val = pow(xi, (double)k);
        if(abs(pow(val,n)-C(1,0))>1e-6) return 0;
    }
    return 1;
}

int ratOK(const vector<double>&P,const vector<double>&Q,int N){
    auto evalp=[&](const vector<double>&A,C x){ C s=0,xp=1; for(double a:A){s+=a*xp; xp*=x;} return s; };
    for(int n=1;n<=N;n++) for(int j=0;j<n;j++){
        C xi=exp(C(0,1)*(2.0*PI*j/n));
        C den=evalp(Q,xi); if(abs(den)<1e-9) return 0;
        C val=evalp(P,xi)/den;
        if(abs(pow(val,n)-C(1,0))>1e-6) return 0;
    }
    return 1;
}

long ordOf(long g,long p){ long o=1,cur=g%p; while(cur!=1){cur=cur*g%p;o++;} return o; }
