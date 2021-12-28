#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
using namespace std;

int main(){
    vector<int> f;
    f.push_back(1);
    vector<int> f_;
    copy(f.begin(),f.end(),back_inserter(f_));
    f_.push_back(0);
    cout << f.size() << endl;
    cout << f_.size() << endl;
    for (int i=0; i<2; i++){
        cout << f_[i] << endl;
    }


    //if(*find(f.begin(),f.end(),1)) cout << "a" << endl;
}