#include <iostream>
#include <algorithm>
#include <ctime>
#include <string>

using namespace std;

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

void spoji(Stek<int> s1, Stek<int> s2, Stek<int>& s3) {
    while(s2.brojElemenata() != 0)
        s3.stavi(s2.skini());
    while(s1.brojElemenata() || s3.brojElemenata()) {
        if(s1.brojElemenata() == 0)
            s2.stavi(s3.skini());
        else if(s3.brojElemenata() == 0)
            s2.stavi(s1.skini());
        else {
            if(s1.vrh() < s3.vrh())
                s2.stavi(s3.skini());
            else
                s2.stavi(s1.skini());
        }
    }
    while(s2.brojElemenata() != 0)
        s3.stavi(s2.skini());
}

int main() {
    Stek<int> s1, s2, s3;
    for(int i = 1; i < 8; i++)
        if(i & 1)
            s1.stavi(i);
        else
            s2.stavi(i);
    spoji(s1, s2, s3);
}