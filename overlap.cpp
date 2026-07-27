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

bool reachablePattern(){
    vector<int> s(n*n);
    for(int i=0;i<n*n;i++) s[i]= bg[i]? -1 : target[i];
    bool changed=true;
    while(changed){
        changed=false;
        for(const auto&p:pairs){
            int a=p.a,b=p.b, sa=s[a], sb=s[b];
            if(sa==0 && sb==0) continue;
            int col = sa>0? sa : sb;
            if((sa==0||sa==col) && (sb==0||sb==col)){
                if(s[a]!=0||s[b]!=0){ s[a]=0; s[b]=0; changed=true; }
            }
        }
    }
    for(int i=0;i<n*n;i++) if(s[i]>0) return false;
    return true;
}

vector<int> matchTo;
bool tryKuhn(int u,const vector<int>&Sc,vector<char>&used){
    for(int d=0;d<4;d++){int r=u/n+DR[d],c=u%n+DC[d]; if(!inb(r,c))continue;
        int v=id(r,c); if(Sc[v]!=Sc[u]||Sc[v]==0) continue;
        if(used[v]) continue; used[v]=1;
        if(matchTo[v]==-1||tryKuhn(matchTo[v],Sc,used)){ matchTo[v]=u; return true; }
    }
    return false;
}
int minDominoesForColour(int col){
    vector<int> Sc(n*n,0); int cnt=0;
    for(int i=0;i<n*n;i++) if(!bg[i]&&target[i]==col){ Sc[i]=col; cnt++; }
    if(cnt==0) return 0;
    matchTo.assign(n*n,-1); int matching=0;
    for(int i=0;i<n*n;i++){
        int r=i/n,c=i%n; if(((r+c)&1)) continue;
        if(Sc[i]!=col) continue;
        vector<char> used(n*n,0);
        if(tryKuhn(i,Sc,used)) matching++;
    }
    return cnt - matching;
}

vector<int> seq;
vector<Move> pos;
unordered_set<string> failMemo;
string enc(int j){
    string s(n*n+4,0);
    for(int i=0;i<n*n;i++) s[i]=(char)(bg[i]?255:st[i]);
    s[n*n]=(char)(j&255); s[n*n+1]=(char)((j>>8)&255);
    return s;
}

bool prune2(int j){
    vector<int> need(k+1,0);
    for(int i=0;i<n*n;i++) if(!bg[i]&&st[i]!=WILD) need[st[i]]++;
    vector<int> have(k+1,0);
    for(int t=1;t<=j;t++) have[seq[t]]++;
    for(int c=1;c<=k;c++) if(need[c] > 2*have[c]) return true;
    static const int dr[4]={0,0,1,-1}, dc[4]={1,-1,0,0};
    for(int r=0;r<n;r++)for(int c=0;c<n;c++){int u=id(r,c);
        if(bg[u]||st[u]==WILD)continue; bool ok=false;
        for(int d=0;d<4;d++){int rr=r+dr[d],cc=c+dc[d];if(inb(rr,cc)&&!bg[id(rr,cc)])ok=true;}
        if(!ok)return true;
    }
    return false;
}

bool dfs2(int j){
    if(++nodes>nodeCap){capHit=true;return false;}
    if(j==0){ for(int i=0;i<n*n;i++) if(!bg[i]&&st[i]!=WILD) return false; return true; }
    if(prune2(j)) return false;
    string key=enc(j);
    if(failMemo.count(key)) return false;
    int c=seq[j];
    vector<const Pair*> order;
    for(const auto&p:pairs){
        int a=p.a,b=p.b;
        if((st[a]==WILD||st[a]==c)&&(st[b]==WILD||st[b]==c)) order.push_back(&p);
    }
    auto progress=[&](const Pair*p){ return (st[p->a]!=WILD)||(st[p->b]!=WILD); };
    stable_sort(order.begin(),order.end(),[&](const Pair*x,const Pair*y){return progress(x)&&!progress(y);});
    for(const Pair* p:order){
        int a=p->a,b=p->b,oa=st[a],ob=st[b];
        st[a]=WILD;st[b]=WILD; pos[j]=p->mv; pos[j].color=c;
        if(dfs2(j-1)) return true;
        st[a]=oa;st[b]=ob;
    }
    if(failMemo.size()<4000000) failMemo.insert(key);
    return false;
}

bool readGrid(){
    target.assign(n*n,0);
    for(int i=0;i<n*n;i++){ if(!(cin>>target[i])) return false; if(target[i]<0||target[i]>k){return false;} }
    bg.assign(n*n,0); for(int i=0;i<n*n;i++) bg[i]=(target[i]==0);
    return true;
}

int countNonBgPairs(){ return (int)pairs.size(); }

bool assembleMode1(vector<Move>& out){
    out.clear();
    long long surplus=0; for(int c=1;c<=k;c++) surplus+=rem[c];
    if(surplus>0){
        if(pairs.empty()) return false;
        Move slot=pairs[0].mv;
        for(int c=1;c<=k;c++) for(int t=0;t<rem[c];t++){ Move mm=slot; mm.color=c; out.push_back(mm);}
    }
    for(int i=(int)revCover.size()-1;i>=0;i--) out.push_back(revCover[i].mv);
    return true;
}

void countConfigs(bool ordered){
    unordered_set<string> seen;
    vector<Move> allPlace;
    for(int r=0;r<n;r++)for(int c=0;c<n;c++){
        if(c+1<n) allPlace.push_back({r,c,true ,0});
        if(r+1<n) allPlace.push_back({r,c,false,0});
    }
    struct St{ vector<int> g; vector<int> rem; int idx; };
    auto gridKey=[&](const vector<int>&g){ string s(n*n,0); for(int i=0;i<n*n;i++)s[i]=(char)g[i]; return s; };
    unordered_set<string> finals;
    vector<St> stk;
    St s0; s0.g.assign(n*n,0);
    if(ordered){ s0.idx=1; } else { s0.rem=rem; s0.idx=0; }
    stk.push_back(s0);
    unordered_set<string> vis;
    long long guard=0;
    while(!stk.empty()){
        if(++guard>20000000LL){ printf("abort\n"); return; }
        St s=stk.back(); stk.pop_back();
        long long remaining = 0;
        if(ordered) remaining = (int)seq.size()-1 - (s.idx-1);
        else { for(int c=1;c<=k;c++) remaining+=s.rem[c]; }
        if(remaining==0){ finals.insert(gridKey(s.g)); continue; }
        string vk; { string g=gridKey(s.g); if(ordered){ vk=g+"#"+to_string(s.idx);} else { g.push_back('#'); for(int c=1;c<=k;c++){g+=to_string(s.rem[c]);g.push_back(',');} vk=g; } }
        if(vis.count(vk)) continue; vis.insert(vk);
        vector<int> colours;
        if(ordered) colours.push_back(seq[s.idx]);
        else for(int c=1;c<=k;c++) if(s.rem[c]>0) colours.push_back(c);
        for(int col:colours){
            for(auto mv:allPlace){
                St t=s; int u=id(mv.r,mv.c),v=mv.horiz?id(mv.r,mv.c+1):id(mv.r+1,mv.c);
                t.g[u]=col; t.g[v]=col;
                if(ordered) t.idx=s.idx+1; else { t.rem[col]--; }
                stk.push_back(t);
            }
        }
    }
    printf("%zu\n", finals.size());
}
