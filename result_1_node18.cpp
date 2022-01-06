#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <time.h>
#include <sys/time.h>
#include <queue>
using namespace std;
using std::ofstream;

const int INF = pow(10,6);
int min_weight, node_num, s, t;
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

void PathAll(int x, int t, vector<int> F, vector<vector<Edge> > G, int sum){
    node_num += 1;
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
                PathAll(y, t, F_copy, G,sum_tmp);
            }
        }
    }
}

int main(){
    string file_name0 = "Graphs/n_18/n_18_m_40.txt";
    string file_name1 = "Graphs/n_18/n_18_m_60.txt";
    string file_name2 = "Graphs/n_18/n_18_m_80.txt";
    string file_name3 = "Graphs/n_18/n_18_m_100.txt";
    string file_name4 = "Graphs/n_18/n_18_m_120.txt";
    //string file_name5 = "Graphs/n_18/n_18_m_140.txt";
    //string file_name6 = "Graphs/n_18/n_18_m_160.txt";
    //string file_name7 = "Graphs/n_18/n_18_m_180.txt";
    //string file_name8 = "Graphs/n_18/n_18_m_30.txt";
    //string file_name9 = "Graphs/n_18/n_18_m_30.txt";

    vector<string> file_name;
    file_name.push_back(file_name0);
    file_name.push_back(file_name1);
    file_name.push_back(file_name2);
    file_name.push_back(file_name3);
    file_name.push_back(file_name4);
    //file_name.push_back(file_name5);
    //file_name.push_back(file_name6);
    //file_name.push_back(file_name7);
    //file_name.push_back(file_name8);
    //file_name.push_back(file_name9);

    int N,M;
    std::ofstream writing_file;
    std::string filename = "kadai1_n_18.txt";
    writing_file.open(filename, std::ios::out);

    for(int _=0; _<file_name.size(); _++){
        int edge_num = _*20 + 40;
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
        PathAll(s,t,F_,G,0);
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