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

vector<int> st;
vector<int> rem;
vector<Pair> revCover;

static const int DR[4]={0,0,1,-1}, DC[4]={1,-1,0,0};

int optionCount(int u){
    int r=u/n,c=u%n,cu=st[u],cnt=0;
    for(int d=0;d<4;d++){int rr=r+DR[d],cc=c+DC[d]; if(!inb(rr,cc))continue;
        int v=id(rr,cc); if(bg[v])continue;
        if(st[v]==WILD||st[v]==cu) cnt++;
    }
    return cnt;
}

int pickMRV(){
    int best=-2, bestCnt=1e9;
    for(int i=0;i<n*n;i++){
        if(bg[i]||st[i]==WILD) continue;
        if(best==-2) best=-3;
        int cnt=optionCount(i);
        if(cnt>0 && cnt<bestCnt){bestCnt=cnt;best=i;}
    }
    if(best==-3) return -2;
    if(best==-2) return -1;
    return best;
}

bool dfs1(){
    if(++nodes>nodeCap){capHit=true;return false;}
    int u=pickMRV();
    if(u==-1) return true;
    if(u==-2) return false;
    int r=u/n,c=u%n,cu=st[u];
    if(rem[cu]==0) return false;
    int cand[4], nc=0;
    for(int d=0;d<4;d++){int rr=r+DR[d],cc=c+DC[d]; if(!inb(rr,cc))continue;
        int v=id(rr,cc); if(bg[v])continue;
        if(st[v]==WILD||st[v]==cu) cand[nc++]=d;
    }
    stable_sort(cand,cand+nc,[&](int x,int y){
        int vx=id(r+DR[x],c+DC[x]), vy=id(r+DR[y],c+DC[y]);
        return (st[vx]==cu) && (st[vy]!=cu);
    });
    for(int t=0;t<nc;t++){
        int d=cand[t], rr=r+DR[d],cc=c+DC[d], v=id(rr,cc), ov=st[v];
        st[u]=WILD; st[v]=WILD; rem[cu]--;
        Move mv = (d==0)?Move{r,c,true ,cu} : (d==1)?Move{r,c-1,true ,cu}
                 :(d==2)?Move{r,c,false,cu} :        Move{r-1,c,false,cu};
        revCover.push_back({u,v,mv});
        if(dfs1()) return true;
        revCover.pop_back();
        st[u]=cu; st[v]=ov; rem[cu]++;
    }
    return false;
}
