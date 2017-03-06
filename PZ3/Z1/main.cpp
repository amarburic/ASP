#include <iostream>
#include <algorithm>
#include <ctime>
#include <utility>

using namespace std;

template <typename Tip>
class Stek {
    vector<Tip> v;
    void TestPraznogSteka() {
        if(v.empty())
            throw "Stek je prazan";
    }
public:
    Stek() : v() {}
    Stek(const Stek& rhs) : v(rhs.v) {}
    Stek(Stek&& rhs) : v(move(rhs.v)) {}
    void brisi() { v.clear(); }
    void stavi(const Tip& el) { v.push_back(el); }
    Tip skini() { 
        TestPraznogSteka();
        auto ret(v.back());
        v.pop_back();
        return ret;
    }
    Tip& vrh() {
        TestPraznogSteka();
        return v.back();
    }
    Tip vrh() const {
        TestPraznogSteka();
        return v.back();
    }
    int brojElemenata() const { return v.size(); }
    Stek& operator=(const Stek& rhs) { 
        if(this == &rhs)
            return *this;
        v = rhs.v;
        return *this;
    }
    Stek& operator=(Stek&& rhs) {       
        if(this == &rhs)
            return *this;
        v = move(rhs.v);
        return *this;}
};/*

template <typename Tip>
class Stek {
    struct Cvor {
        Cvor* prethodni;
        Tip element;
        Cvor(const Tip& element, Cvor* prethodni) :
           prethodni(prethodni), element(element) {}
    };
    int velicina;
    Cvor* pocetak;
    void Obrisi();
    void Kopiraj(const Stek&);
    void TestPraznogSteka() {
        if(pocetak == nullptr)
            throw "Stek je prazan";
    }
public:
    Stek() : velicina(0), pocetak(nullptr) {}
    Stek(const Stek& rhs) : velicina(0), pocetak(nullptr) { Kopiraj(rhs); }
    Stek(Stek&& rhs) : pocetak(rhs.pocetak), velicina(rhs.velicina) { rhs.pocetak = nullptr; }
    ~Stek() { Obrisi(); }
    void brisi() { Obrisi(); }
    void stavi(const Tip& el) {
            pocetak = new Cvor(el, pocetak);
            velicina++; 
    }
    Tip skini();
    Tip& vrh() {
        TestPraznogSteka();
        return pocetak->element;
    }
    Tip vrh() const {
        TestPraznogSteka();
        return pocetak->element;
    }
    int brojElemenata() const { return velicina; };
    Stek& operator=(const Stek&);
    Stek& operator=(Stek&&);
};

template <typename Tip>
void Stek<Tip>::Obrisi() {
    while(pocetak != nullptr) {
        Cvor* temp = pocetak->prethodni;
        delete pocetak;
        pocetak = temp;
    }
    velicina = 0;
}

template <typename Tip>
void Stek<Tip>::Kopiraj(const Stek& rhs) {
    Cvor* temp(nullptr);
    if(rhs.velicina > velicina) {
        Cvor* noviVrh(nullptr), *iter(rhs.pocetak);
        for(int i = 0; i < rhs.velicina - velicina; i++) {
            if(i == 0) {
                noviVrh = new Cvor(iter->element, nullptr);
                temp = noviVrh;
            } else {
                temp->prethodni = new Cvor(iter->element, nullptr);
                temp = temp->prethodni;
            }
            iter = iter->prethodni;
        }
        temp->prethodni = pocetak;
        temp = pocetak;
        pocetak = noviVrh;
    }
    else {
        Cvor* cpy;
        temp = pocetak;
        for(int i = 0; i < rhs.velicina - velicina; i++) {
            cpy = temp->prethodni;
            delete pocetak;
            temp = cpy;
        }
    }
    Cvor* iterTrenutni(temp), *iterNovi(rhs.pocetak);
    for(int i = 0; i < min(rhs.velicina, velicina); i++) {
        iterTrenutni->element = iterNovi->element;
        iterTrenutni = iterTrenutni->prethodni;
        iterNovi = iterNovi->prethodni;
    }
    velicina = rhs.velicina;
}

template <typename Tip>
Tip Stek<Tip>::skini() {
    TestPraznogSteka();
    Tip ret = pocetak->element;
    Cvor* temp = pocetak->prethodni;
    delete pocetak;
    pocetak = temp;
    velicina--;
    return ret; 
}

template <typename Tip>
Stek<Tip>& Stek<Tip>::operator=(const Stek<Tip>& rhs) {
    if(this == &rhs)
        return *this;
    Kopiraj(rhs);
    return *this;
}

template <typename Tip>
Stek<Tip>& Stek<Tip>::operator=(Stek<Tip>&& rhs) {
    if(this == &rhs)
        return *this;
    Kopiraj(rhs);
    rhs.pocetak = nullptr;
    return *this;
}
*/

int main() {
    Stek<int> test;
    clock_t time1 = clock();
    for(int i = 0; i < 1000000; i++)
        test.stavi(i);
    clock_t time2 = clock();
    cout << "Vremenska razlika je: " << (time2 - time1) / (CLOCKS_PER_SEC / 1000.0) << endl;
    Stek<int> test2 = move(test);
}