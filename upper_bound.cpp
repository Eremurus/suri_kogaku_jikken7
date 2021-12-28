#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <time.h>
#include <sys/time.h>
using namespace std;
#include <queue>
//xからtへの部分問題について考える。
//とりあえずxからtへのダイクストラ.負辺は0にする.
//すでにminが決まっている集合(F)に対してはダイクストラしない

const int INF = pow(10,6);

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

int main() {
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
    int s = 3;//始点
    int t = 8;//終点
    F_.push_back(s);

    // ダイクストラ法
    vector<int> dist(N, INF);
    dist[s] = 0;

    // (d[v], v) のペアを要素としたヒープを作る
    priority_queue<pair<int, int>,
                vector<pair<int, int> >,
                greater<pair<int, int> > > que;
    que.push(make_pair(dist[s], s));

    // ダイクストラ法の反復を開始
    while (!que.empty()) {
        // v: 使用済みでない頂点のうち d[v] が最小の頂点
        // d: v に対するキー値
        int v = que.top().second;
        if(v == t) break;
        int d = que.top().first;
        que.pop();

        // d > dist[v] は，(d, v) がゴミであることを意味する
        if (d > dist[v]) continue; 

        // 頂点 v を始点とした各辺を緩和
        for (auto e : G[v]) {
            bool include = false;
            for(i=0; i<F_.size();i++){
                if(e.to == F_[i]) include = true;
            }
            if(include) continue;
            if (chmin(dist[e.to], dist[v] + max(e.w,0))) {
                // 更新があるならヒープに新たに挿入
                dist[e.to] = dist[v] + max(e.w, 0);
                que.push(make_pair(dist[e.to], e.to)); 
            }
        }
    }
    
    // 結果出力
    //for (int v = 0; v < N; ++v) {
        //if (dist[v] < INF) cout << dist[v] << endl;
        //else cout << "INF" << endl;
    //}
    cout << dist[t] << endl;
}