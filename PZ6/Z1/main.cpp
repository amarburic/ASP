#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <fstream>

using namespace std;

template <typename Tip>
void bubble_sort(Tip* niz, int vel) {
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

template <typename Tip>
void selection_sort(Tip* niz, int vel) {
    for(int i = 0; i <= vel - 2; i++) {
        auto min(niz[i]);
        int pmin = i;
        for(int j = i + 1; j <= vel - 1; j++)
            if(niz[j] < min) {
                min = niz[j];
                pmin = j; 
            }
        swap(niz[i], niz[pmin]);
    }
}

template <typename Tip>
int quick_sort_help_particija(Tip* niz, int prvi, int zadnji) {
    auto pivot(niz[prvi]);
    int p(prvi + 1);
    while(p <= zadnji && niz[p] < pivot)
        p++;
    for(int i = p + 1; i <= zadnji; i++)
        if(niz[i] < pivot) {
            swap(niz[p], niz[i]);
            p++;
        }
    swap(niz[prvi], niz[p - 1]);
    return p - 1;
}

template <typename Tip>
void quick_sort_help(Tip* niz, int prvi, int zadnji) {
    if(prvi < zadnji) {
        int j(quick_sort_help_particija(niz, prvi, zadnji));
        quick_sort_help(niz, prvi, j - 1);
        quick_sort_help(niz, j + 1, zadnji);
    }
    
}

template <typename Tip>
void quick_sort(Tip* niz, int vel) {
    quick_sort_help(niz, 0, vel - 1);
}

template <typename Tip>
void merge_sort_help_merge(Tip* niz, int l, int p, int q, int u) {
    vector<Tip> B(niz + l, niz + u + 1);
    int i(0), j(q - l), k(l);
    while(i <= (p - l) && j <= (u - l)) {
        if(B[i] < B[j])
            niz[k++] = B[i++];
        else 
            niz[k++] = B[j++];
    }
    while(i <= (p - l))
        niz[k++] = B[i++];
    while(j <= (u - l))
        niz[k++] = B[j++];    
}

template <typename Tip>
void merge_sort_help(Tip* niz, int pocetak, int kraj) {
    if(pocetak < kraj) {
        int p((pocetak + kraj) / 2), q(p + 1);
        merge_sort_help(niz, pocetak, p);
        merge_sort_help(niz, q, kraj);
        merge_sort_help_merge(niz, pocetak, p, q, kraj);
    }
}

template <typename Tip>
void merge_sort(Tip* niz, int vel) {
    merge_sort_help(niz, 0, vel - 1);
}

void ucitaj(string filename, int*& niz, int &vel) {
    vector<int> temp_vector;
    int temp;
    ifstream buffer(filename);
    if(!buffer)
        throw "Doslo je do problema pri otvaranju datoteke";
    while(buffer >> temp)
        temp_vector.push_back(temp);
    vel = temp_vector.size();
    niz = new int[vel];
    copy(temp_vector.begin(), temp_vector.end(), niz);
}

void zapisi(string filename, int* niz, int vel) {
    ofstream buffer(filename);
    if(!buffer)
        throw "Doslo je do problema pri otvaranju/kreiranju datoteke";
    for(int i = 0; i < vel; i++)
        buffer << niz[i] << " "; 
}

void generisi(string filename, int vel) {
    ofstream buffer(filename);
    if(!buffer)
        throw "Doslo je do problema sa otvaranjem/kreiranjem datoteke";
    srand(time(0));
    for(int i = 0; i < vel; i++)
        buffer << rand() << " ";
}

int main() {
    int* niz, vel;
    bool ponovi;
    
    do {
        ponovi = false;
        cout << "Kako zelite unijeti podatke u niz: " << endl;
        cout << "1) Generisati nasumicno" << endl;
        cout << "2) Ucitati iz fajla " << endl;
        int izbor;
        cin >> izbor;
        switch(izbor) {
            case 1:
            do { 
                try {
                    cout << "Unesite velicinu niza: ";
                    cin >> vel;
                    cout << "Unesite naziv datoteke u koju ce podaci biti smjesteni: ";
                    string path;
                    cin >> path;
                    generisi(path, vel);
                    ucitaj(path, niz, vel);
                } catch(const char* error) {
                    cout << error << endl;
                    ponovi = true;
                }
            } while(ponovi);
            break;
            case 2:
                do {
                    try {
                        ponovi = false;
                        cout << "Unesite path datoteke iz koje se ucitavaju brojevi: ";
                        string temp;
                        cin >> temp;
                        ucitaj(temp, niz, vel);
                    } catch(const char* error) {
                        cout << error << endl;
                        ponovi = true;
                    }
                } while(ponovi);
            break;
            case 3:
                quick_sort(niz, vel);
            break;
            case 4:
                merge_sort(niz, vel);
            break;
            default:
                ponovi = true;
        }
        cout << "Podaci uneseni." << endl;
    } while(ponovi);
    

    
    do {
        ponovi = false;
        cout << "Odaberite algoritam za sortiranje: " << endl;
        cout << "1) Bubble sort: " << endl;
        cout << "2) Selection sort: " << endl;
        cout << "3) Quick sort: " << endl;
        cout << "4) Merge sort: " << endl;
        int izbor;
        cin >> izbor;
        clock_t vrijeme1 = clock();
        switch(izbor) {
            case 1:
                bubble_sort(niz, vel);
            break;
            case 2:
                selection_sort(niz, vel);
            break;
            case 3:
                quick_sort(niz, vel);
            break;
            case 4:
                merge_sort(niz, vel);
            break;
            default:
                ponovi = true;
        }
        if(!ponovi) {
            clock_t vrijeme2 = clock();
            int ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
            cout << "Vrijeme izvrsenja: " << ukvrijeme << " ms." << endl;
        }
    } while(ponovi);
    bool sortiran(true);
    for(int i = 1; i <= vel - 1; i++)
        if(niz[i] < niz[i - 1]) {
            sortiran = false;
            break;
        }
    if(sortiran)
        cout << "Niz je zaista sortiran" << endl;
    else
        cout << "Ups! Niz nije ispravno sortiran" << endl;
    
    do {
        try {
            ponovi = false;
            cout << "Unesite path datoteke u koju ce se spremiti sortiran niz: ";
            string temp;
            cin >> temp;
            zapisi(temp, niz, vel);
        } catch(const char* error) {
            cout << error << endl;
            ponovi = true;
        }
    } while(ponovi);
    cout << "Podaci spaseni." << endl;
}