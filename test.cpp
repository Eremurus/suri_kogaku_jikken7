#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
using namespace std;

struct Edge {
    int to; // 隣接頂点番号
    int w; // 重み
    Edge(int to, long long w) : to(to), w(w) {}
};
int main(){
    vector<Edge> dora;
    vector<Edge> dorara;
    dora.push_back(Edge(1,2));
    dora.push_back(Edge(2,3));
    dora.push_back(Edge(3,4));

    for(auto e : dora){
        dorara.push_back(e);
    }

    for(auto e : dorara){
        cout << e.w << endl;
    }
}