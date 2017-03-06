#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

bool obrnuta(const std::vector<std::vector<std::vector<int>>>& vm) { //O(m * n^2)
    int m(vm.size()), n(vm[0].size());
    vector<vector<bool>> sadrzi_cifru(m, vector<bool>(10));
    for(int i = 0; i < m; i++) {
        int preostalo_cifara(10);
        for(int j = 0; (j < n) && preostalo_cifara; j++) {
            for(int k = 0; (k < n) && preostalo_cifara; k++) {
                auto vr(vm[i][j][k]);
                do {
                    int cifra(vr % 10);
                    if(!sadrzi_cifru[i][cifra])
                        preostalo_cifara--, sadrzi_cifru[i][cifra] = true;
                    vr /= 10;
                } while(vr > 0);
            }
        }
    }
/*
    for(auto vb : sadrzi_cifru) {
        for(auto b : vb ) 
            cout << b << ", ";
        cout << endl;
    }
*/
    for(int i = 0; i < m - 1; i++)
        for(int j = i + 1; j < m; j++) {
            int k(0);
            for(; k < 10; k++)
                if(sadrzi_cifru[i][k] != sadrzi_cifru[j][k])
                    break;
            if(k == 10)
                return true;
        }
    return false;
}

int main() {
    cout << obrnuta({{{15, 3, 25}, {311, 121, 2}, {5, 21, 3112113}}, 
                     {{21, 51, 355}, {15, 3, 25}, {1, 22, 55355}}});
    return 0;
}

