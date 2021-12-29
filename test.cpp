#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
using namespace std;

pair<int, bool> dora(int x){
    pair<int, bool> a;
    a.first = 3;
    a.second = true;
    return a;
}

int main(){
    cout << dora(1).first << endl;
    cout << dora(0).second << endl;
}