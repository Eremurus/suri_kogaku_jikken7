#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
using namespace std;

struct forLowerBound {
    vector<int> path;
} typedef forLowerBound;

int main(){
    vector<int> dora;
    dora.push_back(1);
    dora.push_back(2);
    forLowerBound a;
    a.path = dora;
    for(int i=0; i<a.path.size(); i++){
        cout << a.path[i] << endl;
    }
    //cout << a.path.size() << endl;
}