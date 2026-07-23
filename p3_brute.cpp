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
        }while(next_permutation(tail.begin(),tail.end()));
    }
    return 0;
}
