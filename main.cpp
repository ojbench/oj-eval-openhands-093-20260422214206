// Problem 093 - Czar's Silly Pacman
// Implements right-hand rule navigation with loop detection and scoring

#include <bits/stdc++.h>
using namespace std;

struct State {
    int x, y, dir; // 0:Up,1:Right,2:Down,3:Left
    uint64_t low, high; // pellet bitset 0..63 -> low, 64..99 -> high (36 bits)
    bool operator==(const State& other) const {
        return x==other.x && y==other.y && dir==other.dir && low==other.low && high==other.high;
    }
};

struct StateHash {
    size_t operator()(const State& s) const noexcept {
        size_t h = std::hash<int>{}(s.x);
        h = h * 1315423911u + std::hash<int>{}(s.y);
        h = h * 1315423911u + std::hash<int>{}(s.dir);
        h ^= (std::hash<uint64_t>{}(s.low) + 0x9e3779b97f4a7c15ULL + (h<<6) + (h>>2));
        h ^= (std::hash<uint64_t>{}(s.high) + 0x9e3779b97f4a7c15ULL + (h<<6) + (h>>2));
        return h;
    }
};

static inline bool inb(int x, int y){ return x>=0 && x<10 && y>=0 && y<10; }

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<vector<int>> g(10, vector<int>(10));
    int sx=-1, sy=-1;
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            if(!(cin>>g[i][j])) return 0; // handle incomplete input
            if(g[i][j]==4){ sx=i; sy=j; }
        }
    }
    if(sx==-1){
        // No start found; per constraints this shouldn't happen
        return 0;
    }

    // Pellet bitset
    uint64_t low=0, high=0;
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            if(g[i][j]==2){
                int idx=i*10+j;
                if(idx<64) low |= (1ULL<<idx);
                else high |= (1ULL<<(idx-64));
            }
        }
    }

    // Directions: Up, Right, Down, Left
    int dx[4] = {-1,0,1,0};
    int dy[4] = {0,1,0,-1};
    auto right_of = [](int dir){ return (dir+1)&3; };
    auto left_of  = [](int dir){ return (dir+3)&3; };
    auto back_of  = [](int dir){ return (dir+2)&3; };

    int x=sx, y=sy;
    int dir=0; // initial up
    long long score=0;

    unordered_set<State, StateHash> vis;
    vis.reserve(4096);

    auto pellet_at = [&](int i, int j){
        int idx=i*10+j;
        if(idx<64) return (bool)((low>>idx)&1ULL);
        return (bool)((high>>(idx-64))&1ULL);
    };
    auto clear_pellet = [&](int i, int j){
        int idx=i*10+j;
        if(idx<64) low &= ~(1ULL<<idx);
        else high &= ~(1ULL<<(idx-64));
    };

    // Record initial state
    vis.insert(State{x,y,dir,low,high});

    // Simulation
    const long long STEP_CAP = 2000000; // safety cap
    long long steps=0;
    while(true){
        if(++steps > STEP_CAP){
            cout << "Silly Pacman\n";
            return 0;
        }

        // Determine next direction by priority: right > straight > left > back
        int candidates[4] = { right_of(dir), dir, left_of(dir), back_of(dir) };
        int ndir = -1;
        int nx=0, ny=0;
        for(int k=0;k<4;k++){
            int d=candidates[k];
            int tx=x+dx[d], ty=y+dy[d];
            if(!inb(tx,ty)) continue; // out of bounds treated as wall
            if(g[tx][ty]==0) continue; // wall
            ndir=d; nx=tx; ny=ty; break;
        }
        if(ndir==-1){
            cout << "Silly Pacman\n";
            return 0;
        }

        // Move
        x=nx; y=ny; dir=ndir;

        // Check ghost
        if(g[x][y]==3){
            score -= 500;
            cout << score << "\n";
            return 0;
        }

        // Collect pellet if present
        if(pellet_at(x,y)){
            score += 2;
            clear_pellet(x,y);
        }

        // Loop detection
        State cur{x,y,dir,low,high};
        if(vis.find(cur)!=vis.end()){
            cout << "Silly Pacman\n";
            return 0;
        }
        vis.insert(cur);
    }
}
