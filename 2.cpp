#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <time.h>
#include <sys/time.h>
#include <queue>
using namespace std;

int N, M;
const int INF = pow(10,6);
int min_weight = INF, node_num = 1, s, t;
vector<int> minPath, F_;

double gettimeofday_sec(){
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec+(double) tv.tv_usec*1e-6;
}

// 緩和を実施する関数
bool chmin(int a, int b) {
    if (a > b) {
        return true;
    }
    else return false;
}

struct Edge {
    int to; // 隣接頂点番号
    int w; // 重み
    Edge(int to, long long w) : to(to), w(w) {}
};

int upperBound(int x, int t, vector<int> F, vector<vector<Edge> > G, int sum){
    if(x == t) return 0;
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
            for(int i=0; i<F_.size();i++){
                //if(e.to == F_[i]) include = true;  //////忘れるなよ！！！
            }
            if(include) continue;
            if (chmin(dist[e.to], dist[v] + max(e.w,0))) {
                // 更新があるならヒープに新たに挿入
                dist[e.to] = dist[v] + max(e.w, 0);
                que.push(make_pair(dist[e.to], e.to)); 
            }
        }
    }
    return dist[t];
    //cout << dist[t] << endl;
}

int lowerBound(int x, int t, vector<int> F, vector<vector<Edge> > G, int sum){
    if(x==t) return 0;
    //xからtへの部分問題.閉路を許容した緩和問題とする.
    //ベルマンフォードでN回やる
    //これで本当に下界が求まるか一度試す
    //行けそうなら、「閉路がなければ部分問題の解であるとして打ち切り、経路上の頂点を全てF_に格納」を実装
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
    //for (int v = 0; v < N; ++v) {
    //    if (dist[v] < INF) cout << dist[v] << endl;
    //    else cout << "INF" << endl;
    //}
    return dist[t];
}

void branch_and_bound(int x, int t, vector<int> F, vector<vector<Edge> > G, int sum){
    node_num += 1;
    vector<int> upper_vec;
    vector<int> lower_vec;
    vector<Edge> search_edge;
    for(auto e : G[x]){
        int y = e.to;
        bool include = false;
        for(int i=0; i<F.size(); i++){
            if(F[i] == y) include = true;
        }
        if(include) continue;
        int upper = upperBound(y, t, F, G, sum) + e.w;
        int lower = lowerBound(y, t, F, G, sum) + e.w;
        upper_vec.push_back(upper);
        lower_vec.push_back(lower);
        search_edge.push_back(e);
    }

    for(int i=0; i<upper_vec.size(); i++){
        for(int j=0; j<upper_vec.size(); j++){
            if(upper_vec[i] < lower_vec[j]){
                search_edge.erase(search_edge.begin()+j);
            }
        }
    }

    if(search_edge.size()==0) cout << "search_edgeが空だぞ〜" << endl;
    else{
        for(auto e : search_edge){
            int y = e.to;
            vector<int> F_copy;
            int sum_tmp;
            copy(F.begin(), F.end(),back_inserter(F_copy));
            sum_tmp = sum + e.w;
            F_copy.push_back(y);
            if(y == t){
                    //for(int i=0; i<F_copy.size(); i++){
                        //if(i!= F_copy.size()-1) cout << F_copy[i] << " -> ";
                        //else cout << F_copy[i] << " " << sum_tmp << endl;
                    //}
                    //cout << endl;
                if(sum_tmp < min_weight){
                    min_weight = sum_tmp;
                    minPath.clear();
                    copy(F_copy.begin(), F_copy.end(), back_inserter(minPath));
                }
            }
            else{
                branch_and_bound(y, t, F_copy, G,sum_tmp);
            }
        }
    }
}

int main(){
    string filename("Graphs/n_20/n_20_m_50.txt");
    int number;

    ifstream input_file(filename);
    if (!input_file.is_open()) {
        cerr << "Could not open the file - '"<< filename << "'" << endl;
        return EXIT_FAILURE;
    }

    int i=0;
    vector<int> From, To;
    vector<long long> W;
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
    s = 0;
    t = 10;
    F_.push_back(s);

    double start = gettimeofday_sec();
    branch_and_bound(s,t,F_,G,0);
    double end = gettimeofday_sec();
    std::cout << "実行にかかった時間は " << (end-start)*1000 << " msec"<< endl;
    std::cout << "探索したノードの数は" << node_num <<endl;

    std::cout << "最短経路は" << endl;
    for(int i=0; i<minPath.size(); i++){
        if(i != minPath.size()-1) std::cout << minPath[i] << " -> ";
        else{
            std::cout << minPath[i]<< endl;
        }
    }
    std::cout << "その距離は " << min_weight << endl;
}