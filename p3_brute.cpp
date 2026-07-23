#include <cstdio>
#include <cstdlib>
#include <climits>
#include <vector>
#include <algorithm>
using namespace std;

int main(int argc, char** argv){
    int NMAX = argc>1 ? atoi(argv[1]) : 11;
    printf("n Q1 Q2 Q3 Q4 Q5\n");
    for(int n=2;n<=NMAX;n++){
        vector<int> tail(n-1);
        for(int j=0;j<n-1;j++) tail[j]=j+2;
        long long Q1=LLONG_MIN, Q2=LLONG_MIN;
        long long Q3=LLONG_MAX, Q4=LLONG_MAX, Q5=LLONG_MAX;
        vector<int> sig(n+1), pw(n+1), id(n+1);
        for(int i=1;i<=n;i++) id[i]=i;
        do{
            sig[1]=tail[0];
            for(int j=0;j<n-2;j++) sig[tail[j]]=tail[j+1];
            sig[tail[n-2]]=1;
            vector<int> fres(n,0);
            for(int i=1;i<=n;i++) pw[i]=i;
            fres[0]=0;
            for(int r=1;r<n;r++){
                for(int i=1;i<=n;i++) pw[i]=sig[pw[i]];
                int c=0; for(int i=1;i<=n-1;i++) if(pw[i]==i+1) c++;
                fres[r]=c;
            }
            long long minAll=LLONG_MAX, maxAll=LLONG_MIN;
            long long minOdd=LLONG_MAX, maxOdd=LLONG_MIN;
            long long minEven=LLONG_MAX, maxEven=LLONG_MIN;
            for(int k=1;k<=2*n;k++){
                int r=k%n; int v=fres[r];
                minAll=min(minAll,(long long)v); maxAll=max(maxAll,(long long)v);
                if(k&1){ minOdd=min(minOdd,(long long)v); maxOdd=max(maxOdd,(long long)v);}
                else   { minEven=min(minEven,(long long)v); maxEven=max(maxEven,(long long)v);}
            }
            Q1=max(Q1,minAll);
            Q2=max(Q2,minOdd);
            Q3=min(Q3,maxEven);
            Q4=min(Q4,maxOdd);
            Q5=min(Q5,maxAll);
        }while(next_permutation(tail.begin(),tail.end()));
        printf("%d|%lld|%lld|%lld|%lld|%lld\n",n,Q1,Q2,Q3,Q4,Q5);
    }
    return 0;
}
