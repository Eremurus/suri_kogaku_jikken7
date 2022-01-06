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
int min_weight, node_num, s, t;
vector<int> minPath, F_;

//時間を計る関数
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

//枝を表す
struct Edge {
    int to; // 隣接頂点番号
    int w; // 重み
    Edge(int to, long long w) : to(to), w(w) {}
};

//ベクトルにある要素が含まれているかどうか
bool included(int a, vector<int> vec){
    bool include = false;
    for(int i=0; i<vec.size(); i++){
        if(vec[i]==a){
            include = true;
            break;
        } 
    }
    return include;
}

//x からt への部分問題において上界を計算
int upperBound(int x, int t, vector<int> F, vector<vector<Edge> > G, int sum, int N){
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

        // 頂点 v を始点とした各辺を緩和
        for (auto e : G[v]) {
            if(included(e.to, F)) continue;
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

//下界を求めるための関数.
forLowerBound lowerBound(int x, int t, vector<int> F, vector<vector<Edge> > G, int sum, int N){
    //cout << x<<"に対するlowerが実行されたよ" << endl;
    vector<int> path;
    vector<int> prev(N, -1);
    if(x==t){
        forLowerBound a;
        a.distance = 0;
        a.canCut = true;
        a.path = path;
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
            if(v != x && included(v, F)) continue;

            for (auto e : G[v]) {
                // 緩和処理を行い，更新されたら update を true にする
                if (chmin(dist[e.to], dist[v] + e.w) && !included(e.to, F)) {
                    dist[e.to] = dist[v] + e.w;
                    update = true;
                    prev[e.to] = v;
                }
            }
        }

        // 更新が行われなかったら，すでに最短路が求められている
        if (!update) break;

        // N-F.size() 回目の反復で更新が行われたならば，負閉路をもつ
        if (iter == N-F.size() && update){
            exist_negative_cycle = true;
            break;
        }
    }
    //負閉路を持たない場合、経路を復元
    if(exist_negative_cycle==false && dist[t]!=INF){
        can_cut = true;
        int t_ = t;
        for(; t_!=-1; t_=prev[t_]) path.push_back(t_);
        reverse(path.begin(), path.end());
    }
    forLowerBound a;
    a.distance = dist[t];
    a.canCut = can_cut;
    a.path = path;
    return a;
}

//メイン関数の返す型
struct forBranchAndBound {
    int distance;
    int node;
    vector<int> path;
} typedef forBranchAndBound;

//枝刈りを行う
void branch_and_bound(int x, int t, vector<int> F, vector<vector<Edge> > G, int sum, int N){
    node_num += 1;
    vector<int> upper_vec;
    vector<int> lower_vec;
    vector<Edge> search_edge;
    vector<forBranchAndBound> edge_cut_vec;
    for(auto e : G[x]){
        int y = e.to;
        if(included(y, F)) continue;
        vector<int> F_tmp;
        copy(F.begin(), F.end(), back_inserter(F_tmp));
        F_tmp.push_back(y);
        forLowerBound lower_tmp = lowerBound(y, t, F_tmp, G, sum, N);

        //下界、上界を計算
        int lower = lower_tmp.distance + e.w;
        int upper = upperBound(y, t, F_tmp, G, sum, N) + e.w;

        //下界を求めた際、負閉路がなかった場合
        if(lower_tmp.canCut){
            upper = lower;
            forBranchAndBound b;
            b.distance = lower;
            b.node = y;
            b.path = lower_tmp.path;
            edge_cut_vec.push_back(b);
        }
        upper_vec.push_back(upper);
        lower_vec.push_back(lower);
        search_edge.push_back(e);
    }

    vector<int> erase_num;//枝刈りする枝の行き先

    for(int i=0; i<upper_vec.size(); i++){
        for(int j=0; j<upper_vec.size(); j++){
            if(upper_vec[i] < lower_vec[j] || min_weight<lower_vec[j]+sum){
                erase_num.push_back(j);
            }
        }
    }

    int k=-1;
    //調査する
    if(search_edge.size()!=0){
        for(auto e : search_edge){
            int y = e.to;
            k += 1;  
            bool include = false;
            
            //枝刈りするかどうか
            int I;
            for(int i=0; i<edge_cut_vec.size(); i++){
                if(edge_cut_vec[i].node == y){
                    include = true;
                    I = i;
                }
            }
            
            //下界がその部分問題の最短経路長である場合、枝刈りして暫定の最短距離と比較
            vector<int> F_copy;
            if(include){
                copy(F.begin(), F.end(), back_inserter(F_copy));
                F_copy.insert(F_copy.end(), edge_cut_vec[I].path.begin(), edge_cut_vec[I].path.end());
                int sum_tmp = edge_cut_vec[I].distance+sum;
                if(sum_tmp < min_weight){
                    min_weight = sum_tmp;
                    minPath.clear();
                    copy(F_copy.begin(), F_copy.end(), back_inserter(minPath));
                }
                continue;
            }
            
            //上界 < 下界の時、その下界を与える部分問題を枝刈り
            if(included(k, erase_num)) continue;
            
            int sum_tmp;
            copy(F.begin(), F.end(),back_inserter(F_copy));
            sum_tmp = sum + e.w;
            F_copy.push_back(y);
            //暫定の最短経路長と比較
            if(y == t){
                if(sum_tmp < min_weight){
                    min_weight = sum_tmp;
                    minPath.clear();
                    copy(F_copy.begin(), F_copy.end(), back_inserter(minPath));
                }
            }
            //再帰的に関数を呼び出す
            else{
                branch_and_bound(y, t, F_copy, G, sum_tmp, N);
            }
        }
    }
}


int main(){
    string file_name0 = "Graphs/n_10/n_10_m_20.txt";
    string file_name1 = "Graphs/n_10/n_10_m_30.txt";
    string file_name2 = "Graphs/n_10/n_10_m_40.txt";
    string file_name3 = "Graphs/n_10/n_10_m_50.txt";
    string file_name4 = "Graphs/n_10/n_10_m_60.txt";
    string file_name5 = "Graphs/n_10/n_10_m_70.txt";
    string file_name6 = "Graphs/n_10/n_10_m_80.txt";
    string file_name7 = "Graphs/n_10/n_10_m_90.txt";
    //string file_name8 = "Graphs/n_10/n_10_m_90.txt";
    //string file_name9 = "Graphs/n_10/n_10_m_10.txt";

    vector<string> file_name;
    file_name.push_back(file_name0);
    file_name.push_back(file_name1);
    file_name.push_back(file_name2);
    file_name.push_back(file_name3);
    file_name.push_back(file_name4);
    file_name.push_back(file_name5);
    file_name.push_back(file_name6);
    file_name.push_back(file_name7);
    //file_name.push_back(file_name8);
    //file_name.push_back(file_name9);

    int N,M;
    std::ofstream writing_file;
    std::string filename = "kadai2_n_10.txt";
    writing_file.open(filename, std::ios::out);

    for(int _=0; _<file_name.size(); _++){
        int edge_num = _*10 + 20;
        string filename(file_name[_]);
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
        int s = 0;
        int t = N-1;
        F_.push_back(s);

        node_num = 0;
        min_weight = INF;
        
        double start = gettimeofday_sec();
        branch_and_bound(s,t,F_,G,0,N);
        double end = gettimeofday_sec();
        writing_file << edge_num << "\t"<<(end-start)*1000<<"\t" << node_num << endl;
        //writing_file << "実行にかかった時間は " << (end-start)*1000 << " msec"<< endl;
        //cout << "探索したノードの数は" << node_num <<endl;

        //cout << "最短経路は" << endl;
        /*
        for(int i=0; i<minPath.size(); i++){
            if(i != minPath.size()-1) cout << minPath[i] << " -> ";
            else{
                cout << minPath[i]<< endl;
            }
        }*/
        //cout << "その距離は " << min_weight << endl;
        }
        writing_file.close();
}