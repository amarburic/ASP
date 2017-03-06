#include <iostream>

using namespace std;

template <typename Tip>
void shellSort(Tip* niz, int vel, int* h, int hVel) {
    for(int i = 0; i <= hVel - 1; i++) {
        auto temp(h[i]);
        for(int j = temp; j <= vel - 1; j++) {
            auto temp2(niz[j]);
            int k(j - temp);
            while(k >= 0 && niz[k] > temp2) {
                niz[k + temp] = niz[k];
                k -= temp;
            }
            niz[k + temp] = temp2;
        }
    }
}

int main() {
    int niz[] = {1, 10, 99, 15, 13164, 84, 545};
    int vel((sizeof niz) / (sizeof int()));
    int h[] = {1};
    int hVel((sizeof h) / (sizeof int()));
    shellSort(niz, vel, h, hVel);
    for(int i = 0; i < vel;  i++)
        cout << niz[i] << " "; 
    return 0;
}