#include <iostream>
#include <vector>

using namespace std;

void lokalniMinimum(const vector<vector<int>>& m, int& i, int& j) { //O(n^2)
    int n(m.size());
    vector<vector<int>> offset{{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    for(int ii = 1; i < n - 1; i++)
        for(int jj = 1; j < n - 1; j++) {
            bool is_min(true);
            for(auto p : offset)
                if(m[i][j] > m[i + p[0]][j + p[1]]) {
                    is_min = false; break;
                }
            if(is_min) {
                i = ii, j = jj; return;  
            }
        }
    i = j = -1;
}

int main() {
    return 0;
}

