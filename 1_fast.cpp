#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <time.h>

using namespace std;

struct Edge {
    int to; // 隣接頂点番号
    long long w; // 重み
    Edge(int to, long long w) : to(to), w(w) {}
};

//隣接リスト


//点の遷移を保存
using History = vector<int>;

History Path;
long long total;

int nodes;

int shortest(vector<vector<Edge> >G, History path, int start, int last, long long Lpath){
//startからlastへのG上の最短路で、pathに含まれる点を通らないもの
//Lpathはpathのコスト合計
    for(int i = 0; i < G[start].size(); ++i){
        int flag = 0;
        for(int j = 0; j < path.size(); ++j){
            if(G[start][i].to == path[j]){
                flag = 1;
                break;
            }
        }
        if(flag == 0){
            path.push_back(G[start][i].to);
            Lpath += G[start][i].w;
            if(G[start][i].to == last){
                if(total > Lpath){
                    Path = path;
                    total = Lpath;
                }
            }
            else{
                shortest(G, path, G[start][i].to, last, Lpath);
                path.pop_back();
                Lpath -= G[start][i].w;
            }            
        }
    }
    nodes += 1;
    return 0;
}


int main(void){
	ifstream ifs("Graphs/n_20/n_20_m_100.txt");//読み込むグラフのテキスト
	string str;


	vector<long long> a, b, w;
	long long a_temp, b_temp, w_temp;
    //枝情報

    int N,M;
    //頂点、枝の数

    getline(ifs,str);
    stringstream ss(str);
	ss >> N >> M;

	while (getline(ifs,str)) {
	      stringstream ss(str);
	      ss >> a_temp >> b_temp >> w_temp;
	      a.push_back(a_temp);
	      b.push_back(b_temp);
	      w.push_back(w_temp);
	}	
    //データの読み込み
    vector<vector<Edge> > G(N);
    for (int i = 0; i < M; ++i) {
        G[a[i]].push_back(Edge(b[i], w[i]));
    }
    //グラフ
    vector<int> Path;
    Path.push_back(0);
    total = 10000000000000000;
    nodes = 0;
    vector<int> F;
    //F.push_back(0);

    clock_t startTime = clock();//計測開始
    shortest(G, Path, 0, N-1, 0);
    clock_t endTime = clock();//計測終了
    
    cout<<"Time: "<<(double)(endTime-startTime)*1000/CLOCKS_PER_SEC<<" ms."<<endl;
    
    cout<<"Path: ";
    for(int i = 0; i < Path.size()-1; ++i){
        cout<<Path[i]<<"->";
    }
    cout<<Path[Path.size()-1];
    cout<<endl;

    cout<<"total cost: "<<total<<endl;

    cout<<"nodes: "<<nodes<<endl;
	return 0;
}
