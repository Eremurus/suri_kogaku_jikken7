#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <time.h>
#include <sys/time.h>
#include <queue>
using namespace std;

const int INF = pow(10,6);
int min_weight = INF, node_num = 0, s, t;
vector<int> minPath, F_;

struct Edge {
    int to; // 隣接頂点番号
    int w; // 重み
    Edge(int to, long long w) : to(to), w(w) {}
};

bool chmin(int a, int b) {
    if (a > b) {
        return true;
    }
    else return false;
}

struct forLowerBound {
    int distance;
    bool canCut;
    vector<int> path;
} typedef forLowerBound;

bool included(int a, vector<int> vec){
    bool include = false;
    for(int i=0; i<vec.size(); i++){
        if(vec[i]==a) include = true;
    }
    return include;
}


forLowerBound lowerBound(int x, int t, vector<int> F, vector<vector<Edge> > G, vector<vector<Edge> > G_rev, int sum, int N){
    //cout << x<<"に対するlowerが実行されたよ" << endl;
    vector<int> path_;
    vector<int> prev(N, -1);
    if(x==t){
        forLowerBound a;
        a.distance = 0;
        a.canCut = true;
        a.path = path_;
        return a;
    }

    bool exist_negative_cycle = false; // 負閉路をもつかどうか
    vector<int> dist(N, INF);
    dist[x] = 0;
    bool can_cut = false;
    for (int iter = 0; iter < N-F.size()+1; iter++) {
        bool update = false; // 更新が発生したかどうかを表すフラグ
        for (int v = 0; v < N; ++v) {
            // dist[v] = INF のときは頂点 v からの緩和を行わない
            if (dist[v] == INF) continue;
            bool include = false;
            for(int _=0; _<F.size(); _++){
                if(F[_] == v && v!= x) include = true;
            }
            if(include) continue;

            for (auto e : G[v]) {
                // 緩和処理を行い，更新されたら update を true にする
                if (chmin(dist[e.to], dist[v] + e.w) && e.to!=x && !included(e.to, F)) {
                    dist[e.to] = dist[v] + e.w;
                    update = true;
                    prev[e.to] = v;
                }
            }
        }

        // 更新が行われなかったら，すでに最短路が求められている
        if (!update) break;

        // N 回目の反復で更新が行われたならば，負閉路をもつ
        if (iter == N-F.size() && update){
            exist_negative_cycle = true;
            break;
        }
    }
    //負閉路を持たない場合、経路を復元
    if(exist_negative_cycle==false && dist[t]!=INF){
        can_cut = true;
        for(; t!=-1; t=prev[t]) path_.push_back(t);
        reverse(path_.begin(), path_.end());
    }
    forLowerBound a;
    a.distance = dist[t];
    a.canCut = can_cut;
    a.path = path_;
    return a;
}

