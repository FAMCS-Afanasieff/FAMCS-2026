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
            if(abs(p - C(1,0)) > 1e-8) return 0;
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
        if(abs(pow(val,n)-C(1,0))>1e-8) return 0;
    }
    return 1;
}

int ratOK(const vector<double>&P,const vector<double>&Q,int N){
    auto evalp=[&](const vector<double>&A,C x){ C s=0,xp=1; for(double a:A){s+=a*xp; xp*=x;} return s; };
    for(int n=1;n<=N;n++) for(int j=0;j<n;j++){
        C xi=exp(C(0,1)*(2.0*PI*j/n));
        C den=evalp(Q,xi); if(abs(den)<1e-9) return 0;
        C val=evalp(P,xi)/den;
        if(abs(pow(val,n)-C(1,0))>1e-8) return 0;
    }
    return 1;
}

long ordOf(long g,long p){ long o=1,cur=g%p; while(cur!=1){cur=cur*g%p;o++;} return o; }

int main(){
    int N=15;
    set<string> found;
    for(long a=-4;a<=4;a++)for(long b=-4;b<=4;b++)for(long c=-4;c<=4;c++)for(long d=-4;d<=4;d++){
        if(a*d-b*c==0) continue;
        if(mobiusOK(a,b,c,d,N)) found.insert(classifyMobius(a,b,c,d));
    }
    printf("R(x) =");
    for(auto&s:found) printf(" %s", s.c_str());
    printf("\n");

    printf("x^k:");
    for(int k=-8;k<=8;k++) if(monomialOK(k,N)) printf(" %d",k);
    printf("\n");

    struct T{const char*name; vector<double>P,Q;};
    vector<T> tests={
        {"x^2+1",        {1,0,1},        {1}},
        {"(x^2+x)/2",    {0,1,1},        {2}},
        {"2x",           {0,2},          {1}},
        {"-x",           {0,-1},         {1}},
        {"(x-2)/(1-2x)", {-2,1},         {1,-2}},
        {"(x+1)/2",      {1,1},          {2}},
    };
    for(auto&t:tests) printf("%s:%s\n", t.name, ratOK(t.P,t.Q,N)?"ok":"no");

    for(int p : {3,5,7,11,13}){
        unsigned long long prod=1; for(long g=1;g<p;g++) prod*= (unsigned long long)ordOf(g,p);
        long long brute=-1;
        if(p<=7){
            int u=p-1; vector<long> el; for(long g=1;g<p;g++) el.push_back(g);
            vector<long> ord(p); for(long g=1;g<p;g++) ord[g]=ordOf(g,p);
            vector<int> idx(u,0); brute=0;
            while(true){
                bool ok=true;
                for(int i=0;i<u&&ok;i++){ long xi=el[i], y=el[idx[i]];
                    if(ord[y] % 1 !=0){} if( ord[xi] % ord[y] != 0 ) ok=false; }
                if(ok) brute++;
                int i=0; for(;i<u;i++){ if(++idx[i]<u) break; idx[i]=0; } if(i==u) break;
            }
        }
        printf("%d|%llu|%lld|%d\n", p, prod, brute, p-1);
    }
    return 0;
}
