#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
using namespace std;

int a = 1;
int b = 2;
int upper(int a, int b){
    if(a<b){
        b = a;
    }
    return a;
}
int main(){
    upper(a,b);
    cout << b << endl;
}