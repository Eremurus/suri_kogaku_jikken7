#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <time.h>
#include <sys/time.h>
using namespace std;

double gettimeofday_sec(){
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec+(double) tv.tv_usec*1e-6;
}

struct Edge {
    int to; // 隣接頂点番号
    int w; // 重み
    Edge(int to, int w) : to(to), w(w) {}
};

double min_weight = pow(10,6);
vector<int> minPath;
int node_num = 0;//探索ノード数

//再帰関数
void PathAll(int x, int t, vector<int> F, vector<vector<Edge> > G, int sum){
    node_num += 1;
    //x から伸びる有向枝に対して
    for(auto e : G[x]){
        int y = e.to;
        bool include = false;
        for(int i=0; i<F.size(); i++){
            if(F[i] == y) include = true;
        }
        if(include) continue;
        else{
            vector<int> F_copy;
            int sum_tmp;
            copy(F.begin(), F.end(),back_inserter(F_copy));
            //s からx までの距離にw(x,y) を足す
            sum_tmp = sum + e.w;
            F_copy.push_back(y);
            if(y == t){
                if(sum_tmp < min_weight){
                    min_weight = sum_tmp;
                    minPath.clear();
                    copy(F_copy.begin(), F_copy.end(), back_inserter(minPath));
                }
            }
            else{
                PathAll(y, t, F_copy, G,sum_tmp);
            }
        }
    }
}

int main(){
    //ファイルの読み込み
    int N, M;
    string filename("Graphs/n_14/n_14_m_90.txt");
    int number;

    ifstream input_file(filename);
    if (!input_file.is_open()) {
        cerr << "Could not open the file - '"<< filename << "'" << endl;
        return EXIT_FAILURE;
    }

    //グラフの作成
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

    //時間の計測とpathall の実行
    double start = gettimeofday_sec();
    PathAll(s,t,F_,G,0);
    double end = gettimeofday_sec();
    cout << "実行にかかった時間は " << (end-start)*1000 << " msec"<< endl;
    cout << "探索したノードの数は" << node_num <<endl;

    cout << "最短経路は" << endl;
    for(int i=0; i<minPath.size(); i++){
        if(i != minPath.size()-1) cout << minPath[i] << " -> ";
        else{
            cout << minPath[i]<< endl;
        }
    }
    cout << "その距離は " << min_weight << endl;
}