#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <iostream>

using namespace std;

static const int WILD = 0;

struct Move { int r, c; bool horiz; int color; };

int n, k;
vector<int> target;
vector<char> bg;
vector<int> nbrOffsets;
long long nodeCap = 20000000LL, nodes = 0;
bool capHit = false;

inline int id(int r,int c){ return r*n+c; }
inline bool inb(int r,int c){ return r>=0&&r<n&&c>=0&&c<n; }

struct Pair { int a,b; Move mv; };
vector<Pair> pairs;

void buildPairs(){
    pairs.clear();
    for(int r=0;r<n;r++)for(int c=0;c<n;c++){
        int u=id(r,c); if(bg[u]) continue;
        if(c+1<n && !bg[id(r,c+1)]) pairs.push_back({u,id(r,c+1),{r,c,true ,0}});
        if(r+1<n && !bg[id(r+1,c)]) pairs.push_back({u,id(r+1,c),{r,c,false,0}});
    }
}

vector<int> simulate(const vector<Move>& mv){
    vector<int> g(n*n,0);
    for(const auto&m:mv){
        int u=id(m.r,m.c), v=m.horiz?id(m.r,m.c+1):id(m.r+1,m.c);
        g[u]=m.color; g[v]=m.color;
    }
    return g;
}
bool verify(const vector<Move>& mv){ return simulate(mv)==target; }
