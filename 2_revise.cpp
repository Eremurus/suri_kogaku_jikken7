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

//時間を計る関数
double gettimeofday_sec(){
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec+(double) tv.tv_usec*1e-6;
}

//ベクトルに要素a が含まれているかどうか
bool vec_included(int a, vector<int> vec){
    bool include = false;
    for(int i=0; i<vec.size(); i++){
        if(vec[i] == a) include = true;
    }
    return include;
}

// 緩和を実施する関数
bool chmin(int a, int b) {
    if (a > b) {
        return true;
    }
    else return false;
}

//枝を表す
struct Edge {
    int to; // 隣接頂点番号
    int w; // 重み
    Edge(int to, long long w) : to(to), w(w) {}
};

//x からt への部分問題において上界を計算
int upperBound(int x, int t, vector<int> F, vector<vector<Edge> > G, int sum, int N){
    cout << x<<"に対するupperが実行されたよ" << endl;
    if(x == t) return 0;
    vector<int> dist(N, INF);
    dist[x] = 0;

    // (d[v], v) のペアを要素としたヒープを作る
    priority_queue<pair<int, int>,
                vector<pair<int, int> >,
                greater<pair<int, int> > > que;
    que.push(make_pair(dist[x], x));

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
        //for(int i=0; i<F.size(); i++){
            //cout << F[i] << endl;
        //}

        // 頂点 v を始点とした各辺を緩和
        for (auto e : G[v]) {
            if(vec_included(e.to, F)) continue;
            if (chmin(dist[e.to], dist[v] + max(e.w,0))) {
                // 更新があるならヒープに新たに挿入
                dist[e.to] = dist[v] + max(e.w, 0);
                que.push(make_pair(dist[e.to], e.to)); 
            }
        }
    }
    return dist[t];//上界を返す
}

//下界、負閉路が存在しないかどうか、負閉路が存在しない時の経路(最短路)
struct forLowerBound {
    int distance;
    bool canCut;
    vector<int> path;
} typedef forLowerBound;

//ベルマンフォードで求めた最短路を再構成するための関数.
int make_path(int tmp_node, int x, vector<vector<Edge> > G_rev, vector<int> F, vector<int> dist_){
    for(auto e : G_rev[tmp_node]){
        int tmp_pre = e.to;
        if(vec_included(tmp_pre, F) && tmp_pre!=x){
            cout << e.to<<"への枝はパスしました"<<endl;
            continue;
        }
        cout << "この頂点から調べてるよ:" << tmp_node << endl;
        cout << "この頂点までの距離を調べるよ:" << tmp_pre << endl;
        cout << tmp_node << "までのdistは"<<dist_[tmp_node]<<endl;
        cout << tmp_pre << "までのdistは"<<dist_[tmp_pre] <<endl;
        cout << "2点の間の枝の重さは"<<e.w << endl;
        if(e.w == dist_[tmp_node] - dist_[tmp_pre]){
            //cout << "あ" << endl;
            //cout << tmp_pre << endl;
            cout << tmp_node<<"からの調査は終了だよ"<<endl;
            cout << "次は"<<tmp_pre<<"から調査だね"<<endl;
            return tmp_pre;
            //cout << tmp << endl;
        }
    }
    return INF;
    //cout << "ないで" << endl;
}

//下界を求めるための関数.
forLowerBound lowerBound(int x, int t, vector<int> F, vector<vector<Edge> > G, vector<vector<Edge> > G_rev, int sum, int N){
    //cout << x<<"に対するlowerが実行されたよ" << endl;
    vector<int> path_;
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
    //cout <<"代入したよ"<<endl;
    bool can_cut = false;
    cout << "lower boundを実行するよ.これは"<<x<<"の部分問題だよ" << endl;
    for (int iter = 0; iter < N; iter++) {
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
                bool include_ = false;
                for(int _=0; _<F.size(); _++){
                    if(F[_] == v && v!=x) include_ = true;
                }
                if(include_) continue;
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
        if (iter == N-1 && update){
            exist_negative_cycle = true;
            break;
        }
    }
    //負閉路を持たない場合、経路を復元
    /*
    if(exist_negative_cycle==false){
        cout << x<<"からの部分問題に負閉路はないよ、ここまではできてるよ" << endl;
        
        can_cut = true;
        int tmp_node;
        tmp_node = t;
        path_.insert(path_.begin(), t);
        int tmp_pre_;
        while(true){
            tmp_pre_ = make_path(tmp_node, x, G_rev, F, dist);
            cout << "make_pathが実行されたよ.pathには"<<tmp_pre_<<"が挿入されたね." <<endl;
            //cout << tmp_pre << endl;
            //cout << tmp_pre << endl;
            path_.insert(path_.begin(), tmp_pre_);
            tmp_node = tmp_pre_;
            cout << "実行関数内では次は"<<tmp_node<<"から調べることになってるよ"<<endl;
            if(tmp_node == x) break;
        }
    }*/
    if(exist_negative_cycle==false){
        can_cut = true;
        int tmp = t;
        path_.insert(path_.begin(), t);
        while(tmp!=x){
            for(auto e : G_rev[tmp]){
                int tmp_pre = e.to;
                bool include = false;
                for(int _=0; _<F.size(); _++){
                    if(F[_] == tmp_pre && tmp_pre!=x){
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
    for(int i=0; i<path_.size(); i++){
        cout << path_[i] << endl;
    }
    
    // 結果出力
    //for (int v = 0; v < N; ++v) {
    //    if (dist[v] < INF) cout << dist[v] << endl;
    //    else cout << "INF" << endl;
    //}
    //cout << x << "に対するlowerは"<<dist[t] << endl;
    forLowerBound a;
    a.distance = dist[t];
    a.canCut = can_cut;
    a.path = path_;
    return a;
}

//メイン関数の返す型
struct forBranchAndBound {
    int distance;
    int node;
    vector<int> path;
} typedef forBranchAndBound;


void branch_and_bound(int x, int t, vector<int> F, vector<vector<Edge> > G, vector<vector<Edge> > G_rev, int sum, int N){
    node_num += 1;
    vector<int> upper_vec;
    vector<int> lower_vec;
    vector<Edge> search_edge;
    vector<forBranchAndBound> edge_cut_vec;
    //cout << x << "の部分問題において"<<endl;
    for(auto e : G[x]){
        int y = e.to;
        bool include = false;
        for(int i=0; i<F.size(); i++){
            if(F[i] == y) include = true;
        }
        if(include){
            //cout << y << "は走査済み"<<endl;
            continue;
        }
        vector<int> F_tmp;
        copy(F.begin(), F.end(), back_inserter(F_tmp));
        F_tmp.push_back(y);
        int upper = upperBound(y, t, F_tmp, G, sum, N) + e.w;
        //cout << upperBound(y, t, F_tmp, G, sum, N) << endl;
        forLowerBound lower_tmp = lowerBound(y, t, F_tmp, G, G_rev, sum, N);
        int lower = lower_tmp.distance + e.w;
        vector<int> path__;
        copy(lower_tmp.path.begin(), lower_tmp.path.end(), back_inserter(path__));
        //cout << x<<"と"<<y<<"の間の枝をとおる.tへの経路の上界は"<<upper << "で、下界は"<<lower<<endl;
        if(lower_tmp.canCut){
            upper = lower;
            forBranchAndBound b;
            b.distance = lower;
            b.node = y;
            b.path = lower_tmp.path;
            edge_cut_vec.push_back(b);
            //continue;//何かの配列にyと経路とlowerを格納.
        }
        upper_vec.push_back(upper);
        lower_vec.push_back(lower);
        search_edge.push_back(e);
        //cout << y <<"をsearch_edgeに格納"<<endl;
    }

    vector<int> erase_num;

    for(int i=0; i<upper_vec.size(); i++){
        for(int j=0; j<upper_vec.size(); j++){
            if(upper_vec[i] < lower_vec[j]){
                erase_num.push_back(j);
                //cout << search_edge[j].to << "への枝は枝切りする予定です."<<search_edge[i].to<<"を通る経路の上界は"<<upper_vec[i]<<"で、"<<search_edge[j].to<<"を通る経路の下界は"<<lower_vec[j]<<endl;
            }
        }
    }
    int k=-1;
    if(search_edge.size()!=0){
        for(auto e : search_edge){
            int y = e.to;
            k += 1;  
            bool include = false;
            
            int I;
            for(int i=0; i<edge_cut_vec.size(); i++){
                if(edge_cut_vec[i].node == y){
                    include = true;
                    I = i;
                }
            }
            
            vector<int> F_copy;
            if(include){
                copy(F.begin(), F.end(), back_inserter(F_copy));
                F_copy.insert(F_copy.end(), edge_cut_vec[I].path.begin(), edge_cut_vec[I].path.end());
                int sum_tmp = edge_cut_vec[I].distance+sum;
                /*
                for(int i=0; i<F_copy.size(); i++){
                    cout << F_copy[i] << endl;
                }
                */
                if(sum_tmp < min_weight){
                    min_weight = sum_tmp;
                    minPath.clear();
                    copy(F_copy.begin(), F_copy.end(), back_inserter(minPath));
                }
                continue;
            }
            //もし配列にyが含まれていたら、F_copyにF+経路をコピーしてsum+lowerをmin_weightと比較.代入するか破棄するかして、continue.
            //cout <<x<<"の部分問題において"<< y << "への枝を調べる可能性があります." << endl;
            
            
            include = false;
            for(int i=0; i<erase_num.size(); i++){
                if(erase_num[i]==k) include=true;
            }
            if(include){
                //cout << y << "への枝は枝刈りしました"<<endl;
                continue;
            }
            
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
                branch_and_bound(y, t, F_copy, G, G_rev, sum_tmp, N);
            }
        }
    }
}

int main(){
    string filename("Graphs/n_6/n_6_m_12.txt");
    int number;

    ifstream input_file(filename);
    if (!input_file.is_open()) {
        cerr << "Could not open the file - '"<< filename << "'" << endl;
        return EXIT_FAILURE;
    }

    int i=0;
    vector<int> From, To;
    vector<long long> W;
    int N, M;
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
    s = 2;
    t = 4;
    F_.push_back(s);

    double start = gettimeofday_sec();
    branch_and_bound(s,t,F_,G,G_rev, 0,N);
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