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
    string filename("Graphs/n_20/n_20_m_50.txt");
    int number;

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

    for(int k=0; k<M; k++){
        int from = From[k];
        int to = To[k];
        int w = W[k];
        G[from].push_back(Edge(to, w));
        }
    vector<int> F_;
    int s = 0;//始点
    int t = 3;//終点
    F_.push_back(s);
    bool exist_negative_cycle = false; // 負閉路をもつかどうか
    vector<int> dist(N, INF);
    dist[s] = 0;
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

    if(exist_negative_cycle==false){
        //その部分問題の最短経路長を表す変数にdist[t]を代入.F_に最短経路上の頂点を格納.上界、下界を求め終わりpathallする際にF_が使われるのでこれで打ち切りができている.
    }
    // 結果出力
    for (int v = 0; v < N; ++v) {
        if (dist[v] < INF) cout << dist[v] << endl;
        else cout << "INF" << endl;
    }
}