#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>

using namespace std;

void zapisi(string filename, int* niz, int vel) {
    ofstream buffer(filename);
    if(!buffer)
        throw "Doslo je do problema pri otvaranju/kreiranju datoteke";
    for(int i = 0; i < vel; i++)
        buffer << niz[i] << " "; 
}

void generisiZaBubble(string filename, int vel) {
    vector<int> niz(vel);
    srand(time(0));
    int baza(6);
    for(int i = 0; i < vel; i++)
        niz[i] = (baza * i) + (rand() % baza);
    zapisi(filename, &niz[0], vel);
}

template <typename Tip>
void modifikovani_bubble_sort(Tip* niz, int vel) {
    while(vel != 0) {
        int i(vel - 1);
        vel = 0;
        for(int j = 1; j <= i; j++)
            if(niz[j] < niz[j - 1]) {
                swap(niz[j], niz[j - 1]);
                vel = j;
            }
    }
}

int main() {
    generisiZaBubble("test.txt", 10);
    return 0;
}