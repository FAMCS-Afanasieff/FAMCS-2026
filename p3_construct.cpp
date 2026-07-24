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
        printf("%d\n", n);
    }
    printf("%s\n", allok?"OK":"FAILURE");
    return 0;
}
