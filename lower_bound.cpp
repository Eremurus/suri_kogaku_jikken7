#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <time.h>
#include <sys/time.h>
using namespace std;
#include <queue>

//xからtへの部分問題.閉路を許容した緩和問題とする.
//ベルマンフォードでN回やる
//これで本当に下界が求まるか一度試す
//行けそうなら、「閉路がなければ部分問題の解であるとして打ち切り、経路上の頂点を全てF_に格納」を実装

const int INF = pow(10,6);
vector<int> F;

// 辺を表す型，ここでは重みを表す型を long long 型とします
struct Edge {
    int to; // 隣接頂点番号
    int w; // 重み
    Edge(int to, int w) : to(to), w(w) {}
};

// 重み付きグラフを表す型
int N;

// 緩和を実施する関数
bool chmin(int a, int b) {
    if (a > b) {
        return true;
    }
    else return false;
}

int main(){
    int N, M;
    string filename("Graphs/test.txt");
    int number;
    bool cancut = false;

    ifstream input_file(filename);
    if (!input_file.is_open()) {
        cerr << "Could not open the file - '"<< filename << "'" << endl;
        return EXIT_FAILURE;
    }

    int i=0;
    vector<int> From, To, W;
    while (input_file >> number) {
        i++;
        if(i==1) N = number;
        else if(i==2) M = number;
        else{
            if (i%3==0) From.push_back(number);
            if (i%3==1) To.push_back(number);
            if (i%3==2) W.push_back(number);
        }
    }
    input_file.close();

    vector<vector<Edge> > G(N);
    vector<vector<Edge> > G_rev(N);

    for(int k=0; k<M; k++){
        int from = From[k];
        int to = To[k];
        int w = W[k];
        G[from].push_back(Edge(to, w));
        G_rev[to].push_back(Edge(from, w));
        }
    vector<int> F_;
    int s = 0;//始点
    int t = 4;//終点
    F_.push_back(s);

    bool exist_negative_cycle = false; // 負閉路をもつかどうか
    vector<int> dist(N, INF);
    dist[s] = 0;
    vector<int> path_;
    for (int iter = 0; iter < N; ++iter) {
        bool update = false; // 更新が発生したかどうかを表すフラグ
        for (int v = 0; v < N; ++v) {
            // dist[v] = INF のときは頂点 v からの緩和を行わない
            if (dist[v] == INF) continue;
            
            for (auto e : G[v]) {
                // 緩和処理を行い，更新されたら update を true にする
                if (chmin(dist[e.to], dist[v] + e.w)) {
                    dist[e.to] = dist[v] + e.w;
                    update = true;
                }
            }
        }

        // 更新が行われなかったら，すでに最短路が求められている
        if (!update) break;

        // N 回目の反復で更新が行われたならば，負閉路をもつ
        if (iter == N - 1 && update){
            exist_negative_cycle = true;
            break;
        }
    }
/*
for(int i=0; i<N; i++){
    cout << dist[i] << endl;
}
*/
    if(exist_negative_cycle==false){
        cancut = true;
        int tmp = t;
        path_.insert(path_.begin(), t);
        while(tmp!=s){
            for(auto e : G_rev[tmp]){
                int tmp_pre = e.to;
                bool include = false;
                for(int _=0; _<F.size(); _++){
                    if(F[_] == tmp_pre && tmp_pre!=s){
                        include = true;
                    }
                }
                if(include) continue;
                if(e.w == dist[tmp] - dist[tmp_pre]){
                    path_.insert(path_.begin(), tmp_pre);
                    tmp = tmp_pre;
                }
            }
        }    
    }
    // 結果出力
    for(int i=0; i<path_.size(); i++){
        cout << path_[i] << endl;
    }
}