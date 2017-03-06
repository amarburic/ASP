#include <iostream>
#include <vector>
#include <cmath>
#include <iterator>
#include <algorithm>

using namespace std;

void radixSort(vector<int> &a) {
    vector<vector<int>> q(10);
    int max_cifara(0);
    for(int i : a)
        max_cifara = max((int)ceil(log10(i)), max_cifara);
    int upper(10), lower(1);
    for(int j = 1; j <= max_cifara; j++) {
        for(int i = 0; i < a.size(); i++) 
            q[a[i] % upper / lower].push_back(a[i]);
        a.clear();
        for(auto& vi : q) {
            copy(vi.begin(), vi.end(), back_inserter(a));
            vi.clear();
        }
        upper *= 10; lower *= 10;
    }
}

int lijevoDijete(int i) {
    return 2 * i + 1;
}

int desnoDijete(int i) {
    return 2 * i + 2;
}

int roditelj(int i) {
    return (i - 1) / 2;
} 

bool jeLiList(vector<int> &a, int i, int velicina) {
    return (i >= (velicina / 2)) && (i < velicina);
}

void popraviGore(vector<int> &a, int i) {
    if(i != 0) 
        if(a[roditelj(i)] < a[i]) {
            swap(a[i], a[roditelj(i)]);
            popraviGore(a, roditelj(i));
        }
}

void popraviDolje(vector<int> &a, int i, int velicina) { 
    if(!jeLiList(a, i, velicina)) {
        int veci(lijevoDijete(i));
        if(desnoDijete(i) < velicina)
            if(a[veci] < a[desnoDijete(i)])
                veci = desnoDijete(i);
        if(a[veci] > a[i]) {
            swap(a[i], a[veci]);
            popraviDolje(a, veci, velicina);
        }
    }
}

void stvoriGomilu(vector<int> &a) {
    for(int i = a.size() / 2; i >= 0; i--)
        popraviDolje(a, i, a.size());
}

void umetniUGomilu(vector<int> &a, int umetnuti, int &velicina) {
    if(a.size() == velicina)
        a.push_back(0);
    a[velicina++] = umetnuti;
    popraviGore(a, velicina - 1);
}

int izbaciPrvi(vector<int> &a, int &velicina) {
    if(velicina == 0)
        throw "Prazna gomila";
    swap(a[0], a[--velicina]);
    if(velicina != 0)
        popraviDolje(a, 0, velicina);
    return a[velicina];
}

void gomilaSort(vector<int> &a) {
    stvoriGomilu(a);
    int velicina(a.size());
    for(int i = 0; i <= a.size() - 2; i++)
        izbaciPrvi(a, velicina);
}

int main() {

    return 0;
}