#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
using namespace std;

int main(int argc, char** argv){
    int NLO = argc>1?atoi(argv[1]):2;
    int NHI = argc>2?atoi(argv[2]):40;
    bool allok=true;
    for(int n=NLO;n<=NHI;n++){
        vector<int> S(n);
        S[0]=0;
        for(int i=1;i<n;i++){
            int t=(i+1)/2;
            int val = (i%2==1)? t : (n - t)%n;
            S[i]= ((val%n)+n)%n;
        }
        vector<int> seen(n,0); bool dist=true;
        for(int i=0;i<n;i++){ if(seen[S[i]]++){dist=false;} }
        vector<int> m(n,0);
        for(int i=1;i<n;i++){ int d=((S[i]-S[i-1])%n+n)%n; m[d]++; }
        vector<int> elemAtPos(n,0);
        elemAtPos[0]=1;
        for(int i=1;i<n;i++) elemAtPos[S[i]]=i+1;
        vector<int> posOf(n+1,0);
        for(int p=0;p<n;p++) posOf[elemAtPos[p]]=p;
        vector<int> sig(n+1);
        for(int x=1;x<=n;x++) sig[x]=elemAtPos[(posOf[x]+1)%n];
        vector<int> pw(n+1); for(int i=1;i<=n;i++) pw[i]=i;
        bool fmatch=true;
        for(int r=1;r<n;r++){
            for(int i=1;i<=n;i++) pw[i]=sig[pw[i]];
            int c=0; for(int i=1;i<=n-1;i++) if(pw[i]==i+1) c++;
            if(c!=m[r]) fmatch=false;
        }
        int maxAll=0; for(int r=1;r<n;r++) maxAll=max(maxAll,m[r]);
        int expected = (n%2==0)?2:2;
        bool ok = dist && fmatch && (maxAll==expected);
        allok = allok && ok;
        printf("%d %d", n, maxAll);
        if(!ok) printf(" FAIL");
        printf("\n");
    }
    printf("%s\n", allok?"OK":"FAILURE");
    return 0;
}
