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
        printf("%d\n", n);
    }
    return 0;
}
