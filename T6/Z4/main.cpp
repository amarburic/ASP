#include <iostream>
#include <vector>

using namespace std;

template <typename Tip>
void countingSort(Tip* a, int vel, int* bNiz, int k) {
    vector<Tip> c(k + 1);
    for(int j = 0; j < vel; j++)
        c[a[j]]++;
    for(int i = 1; i <= k; i++)
        c[i] += c[i - 1];
    for(int j = vel - 1; j >= 0; j--) {
        c[a[j]]--;
        bNiz[c[a[j]]] = a[j];
    }
}

int main() {
    int niz[] = {1, 10, 99, 15, 13164, 84, 545};
    int vel((sizeof niz) / (sizeof int()));
    int* h = new int[vel];
    countingSort(niz, vel, h, 13164);
    for(int i = 0; i < vel;  i++)
        cout << h[i] << " "; 
    delete[] h;
    return 0;
}